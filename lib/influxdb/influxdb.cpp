#include "influxdb.h"

#if defined(ESP32)
#define DEVICE "ESP32"
WiFiMulti wifiMulti;
#endif

#define WIFI_SSID "NCKU-Wifi"
#define WIFI_PASSWORD "91Y1191Y11"
#define channel 10        // WiFi Channel number between 1 and 13
#define hide_SSID false   // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
#define max_connection 2  // Maximum simultaneous connected clients on the AP
// #define wifi_init()  MODE##_init()

#if MODE == 0
  #define wifi_init() AP_init()
#elif MODE == 1
  #define wifi_init() STA_init()
#endif

static inline void AP_init();
static inline void STA_init();

#define INFLUXDB_URL "http://192.168.0.129:8086"
#define INFLUXDB_TOKEN "LYKHs5H461Y2rvNP5DjCtG3Wf0L4OPoVEjCtpCE9HE3Pq2p7S2-OS2tBGHYKF1IlIvMdjvN40I0W1tD_skWqdw=="
#define INFLUXDB_ORG "NCKU"
#define INFLUXDB_BUCKET "Data"

// Time zone info
#define TZ_INFO "UTC8"

// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
static InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data point
static Point sensor("lora_data");
static lora_data_t *lora_data;

void influxdb_init() {
  lora_data = lora_data_fetch();
  wifi_init();

  // Check server connection
  if (client.validateConnection()) {
    printf("Connected to InfluxDB: %s\n", (String)client.getServerUrl());
  } else {
    printf("InfluxDB connection failed: %s\n", (String)client.getLastErrorMessage());
  }

  // Add tags to the data point
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());
}

void influxdb_task(void *) {
  for (;;) {
    // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();

    // Store measured value into point
    // Report RSSI of currently connected network
    sensor.addField("rssi", WiFi.RSSI());
    sensor.addField("state", lora_data->state);
    sensor.addField("systick", lora_data->systick);
    sensor.addField("pressure_altitude", lora_data->pressure_altitude);
    sensor.addField("pressure_velocity", lora_data->pressure_velocity);
    sensor.addField("longitude", lora_data->longitude);
    sensor.addField("latitude", lora_data->latitude);
    sensor.addField("gps_altitude", lora_data->gps_altitude);
    sensor.addField("acceleration.x", lora_data->acceleration.x);
    sensor.addField("acceleration.y", lora_data->acceleration.y);
    sensor.addField("acceleration.z", lora_data->acceleration.z);
    sensor.addField("gyro.x", lora_data->gyro.x);
    sensor.addField("gyro.y", lora_data->gyro.y);
    sensor.addField("gyro.z", lora_data->gyro.z);
    sensor.addField("roll", lora_data->roll);
    sensor.addField("pitch", lora_data->pitch);
    sensor.addField("heading", lora_data->heading);

    // Print what are we exactly writing
    // printf("Writing: %s\n", sensor.toLineProtocol());

    // Write point (This is the line that write data to DB)
    if (!(client.writePoint(sensor))) {
      printf("InfluxDB write failed: %s\n", (String)client.getLastErrorMessage());
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

static inline void STA_init() {
  // Setup wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  printf("Connecting to wifi\n");
  while (wifiMulti.run() != WL_CONNECTED) {
    printf(".\n");
    delay(100);
  }

  // Accurate time is necessary for certificate validation and writing in batches
  // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
}

static inline void AP_init() {
  printf("\n[*] Creating AP\n");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, channel, hide_SSID, max_connection);
  printf("[+] AP Created with IP Gateway %s\n", (String)WiFi.softAPIP());
}