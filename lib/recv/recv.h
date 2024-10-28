#ifndef _RECV_
#define _RECV_

#include <stdint.h>

#include "Arduino.h"
#include "ra01s.h"

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
} lora_data_t;

void recv_task(SX126x *);
lora_data_t *lora_data_fetch();
void recv_print();

#endif