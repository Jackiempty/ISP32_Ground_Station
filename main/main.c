#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "bsp.h"
#include "ra01s.h"
#include "recv.h"

void app_main() {
  // Initialization
  spi_init(LORA_SPI_HOST, CONFIG_LORA_MOSI_GPIO, CONFIG_LORA_MISO_GPIO, CONFIG_LORA_SCK_GPIO);
  lora_init();

  // Enter task
  printf("I am a ground receiver board!\n");
  vTaskDelay(pdMS_TO_TICKS(1000));
  xTaskCreatePinnedToCore(recv_task, "recv_task", 8192, NULL, 5, NULL, 1); 

  printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
}