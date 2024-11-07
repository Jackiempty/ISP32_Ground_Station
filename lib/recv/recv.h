#ifndef _RECV_
#define _RECV_

#include <stdint.h>
#include <stdio.h>

#include "Arduino.h"
#include "ra01s.h"

#define RF_FREQUENCY 433000000  // Hz  center frequency
// #define RF_FREQUENCY 866000000  // Hz  center frequency
// #define RF_FREQUENCY 915000000  // Hz  center frequency
#define TX_OUTPUT_POWER 22  // dBm tx output power
#define LORA_BANDWIDTH 4    // bandwidth
                            // 2: 31.25Khz
                            // 3: 62.5Khz
                            // 4: 125Khz
                            // 5: 250KHZ
                            // 6: 500Khz
#define LORA_SPREADING_FACTOR 7  // spreading factor [SF5..SF12]
#define LORA_CODINGRATE 1   // [1: 4/5,
                            //  2: 4/6,
                            //  3: 4/7,
                            //  4: 4/8]

#define LORA_PREAMBLE_LENGTH 8  // Same for Tx and Rx
#define LORA_PAYLOADLENGTH 0  // 0: Variable length packet (explicit header)
                              // 1..255  Fixed length packet (implicit header)
                              
#define CONFIG_LORA_MOSI_GPIO 35
#define CONFIG_LORA_MISO_GPIO 37
#define CONFIG_LORA_SCK_GPIO 36
#define CONFIG_LORA_NSS_GPIO 21
#define CONFIG_RST_GPIO 41
#define CONFIG_BUSY_GPIO 40
#define CONFIG_TXEN_GPIO 0
#define CONFIG_RXEN_GPIO 0

typedef enum {
  FSM_INIT,
  FSM_GROUND,
  FSM_RASING,
  FSM_APOGEE,
  FSM_PARACHUTE,
  FSM_DETACH,
  FSM_FALING,
  FSM_DEPLOY,
  FSM_TOUCH_GROUND,
  FSM_END,
} fsm_state_e;

typedef struct {
  float x, y, z;
} vector_t;

typedef struct {
  fsm_state_e state;
  uint32_t systick;
  float pressure_altitude;
  float pressure_velocity;
  vector_t acceleration;
  vector_t gyro;
  int32_t latitude;
  int32_t longitude;
  float gps_altitude;
  float heading, pitch, roll;
  int8_t rssi, snr;
} lora_data_t;

void recv_init();
void recv_task(void *);
lora_data_t *lora_data_fetch();
void recv_print();

#endif