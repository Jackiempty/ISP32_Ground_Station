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
  // Initialization
  // influxdb_init();
  delay(500);
  printf("Start init.......\n");
  recv_init();
  printf("Enable TCXO\n");
  int16_t ret = lora.begin(RF_FREQUENCY,       // frequency in Hz
                           TX_OUTPUT_POWER,    // tx power in dBm
                           3.3,                // use TCXO
                           true);              // use TCXO
  if (ret != ERR_NONE) while(1) {delay(1);}
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
  delay(500);
}