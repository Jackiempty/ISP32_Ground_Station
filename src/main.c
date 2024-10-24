#include <WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "bsp.h"
#include "ra01s.h"
#include "recv.h"

void setup(){
  Serial.begin(115200);
  // Initialization
  spi_init(LORA_SPI_HOST, CONFIG_LORA_MOSI_GPIO, CONFIG_LORA_MISO_GPIO, CONFIG_LORA_SCK_GPIO);
  lora_init();

  printf("I am a ground receiver board!\n");
}

void loop() {
  recv_task();
}