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

void recv_task(SX126x*);
