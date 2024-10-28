#include "influxdb.h"

#include "recv.h"

#if defined(ESP32)
#define DEVICE "ESP32"
WiFiMulti wifiMulti;
#endif

#define WIFI_SSID "Room_0562"
#define WIFI_PASSWORD "7150026666"
#define channel 10               // WiFi Channel number between 1 and 13
#define hide_SSID false         // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
#define max_connection 2         // Maximum simultaneous connected clients on the AP
#define wifi_init() MODE##_init()

static inline void AP_init();
static inline void STA_init();

#define INFLUXDB_URL "http://192.168.4.2:8086"
#define INFLUXDB_TOKEN "uK_5CudAdTeRoci5K8fgyFwRMCX5VhJluAPu11tI6LrnBL86bwsr0DDQC4hxF8FJsCvFfv--IX3d-SIxd2EyJw=="
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
  STA_init();

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  // Add tags to the data point
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());
}

void influxdb_task() {
  // Clear fields for reusing the point. Tags will remain the same as set above.
  sensor.clearFields();
  // sin_wave.clearFields();

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
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  // Write point (This is the line that write data to DB)
  if (!(client.writePoint(sensor))) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  Serial.println("==========");

  delay(1000);
}

static inline void STA_init() {
  // Setup wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Accurate time is necessary for certificate validation and writing in batches
  // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
}

static inline void AP_init() {
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, channel, hide_SSID, max_connection);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());
}