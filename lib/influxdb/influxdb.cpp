#if defined(ESP32)
#define DEVICE "ESP32"
#endif
#include "influxdb.h"

const char* ssid           = "esp_32_AP";                // SSID Name
const char* password       = "ispforever";   // SSID Password - Set to NULL to have an open AP
const int   channel        = 10;                        // WiFi Channel number between 1 and 13
const bool  hide_SSID      = false;                     // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
const int   max_connection = 2;                         // Maximum simultaneous connected clients on the AP

#define INFLUXDB_URL "http://192.168.4.2:8086"
#define INFLUXDB_TOKEN "uK_5CudAdTeRoci5K8fgyFwRMCX5VhJluAPu11tI6LrnBL86bwsr0DDQC4hxF8FJsCvFfv--IX3d-SIxd2EyJw=="
#define INFLUXDB_ORG "NCKU"
#define INFLUXDB_BUCKET "Data"

// Time zone info
#define TZ_INFO "UTC8"

// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data point
Point sensor("wifi_status");

void influxdb_init() {
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, channel, hide_SSID, max_connection);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());

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
  sensor.addTag("SSID", "ESP32_AP");
}

void influxdb_task() {
  // Clear fields for reusing the point. Tags will remain the same as set above.
  sensor.clearFields();
  // sin_wave.clearFields();

  // Store measured value into point
  // Report RSSI of currently connected network
  sensor.addField("rssi", WiFi.RSSI());

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