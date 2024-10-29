#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFi.h>
#include <influxdb.h>
#include <ra01s.h>
#include <recv.h>

static SX126x lora(CONFIG_LORA_NSS_GPIO,  // Port-Pin Output: SPI select
                   CONFIG_RST_GPIO,       // Port-Pin Output: Reset
                   CONFIG_BUSY_GPIO,      // Port-Pin Input:  Busy
                   CONFIG_TXEN_GPIO,      // Port-Pin Output: TXEN
                   CONFIG_RXEN_GPIO       // Port-Pin Output: RXEN
);

void setup() {
  Serial.begin(115200);
  // Initialization
  // influxdb_init();
  lora.LoRaConfig(LORA_SPREADING_FACTOR, 
                  LORA_BANDWIDTH, 
                  LORA_CODINGRATE, 
                  LORA_PREAMBLE_LENGTH, 
                  LORA_PAYLOADLENGTH, 
                  true,               //crcOn  
                  false);             //invertIrq

  printf("I am a ground receiver board!\n");
}

void loop() {
  recv_task(&lora);
  // influxdb_task();
  recv_print();
}