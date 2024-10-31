#include <Arduino.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFi.h>
#include <influxdb.h>
#include <recv.h>

void setup() {
  // Initialization
  delay(500);
  printf("Start init.......\n");
  recv_init();
  influxdb_init();
  printf("------------ Finish init ------------\n");
  printf("I am a ground receiver board!\n");

  xTaskCreate(recv_task, "recv_task", 4096, NULL, 5, NULL);
  xTaskCreate(influxdb_task, "influxdb_task", 4096, NULL, 4, NULL);
}

void loop() {
  // main code
}