#include "bsp.h"
#include "ra01s.h"
#include "recv.h"

void app_main() {
  spi_init(LORA_SPI_HOST, CONFIG_LORA_MOSI_GPIO, CONFIG_LORA_MISO_GPIO, CONFIG_LORA_SCK_GPIO);

}