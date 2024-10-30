#include "recv.h"

#include <stddef.h>
#include <stdint.h>

#define TAG "RECV"
#define RECV_LEN 255

static SX126x *Lora;
static lora_data_t lora_data;
static uint8_t data[RECV_LEN];
static size_t len;
static uint8_t led_status;

static fsm_state_e *state;
static uint32_t *systick;
static float *pressure_altitude;
static float *pressure_velocity;
static vector_t *acceleration;
static vector_t *gyro;
static int32_t *latitude;
static int32_t *longitude;
static float *gps_altitude;
static float *heading;
static float *pitch;
static float *roll;

void recv_init(SX126x *lora) {
  led_status = 0;

  Lora = lora;
  state = &lora_data.state;
  systick = &lora_data.systick;
  pressure_altitude = &lora_data.pressure_altitude;
  pressure_velocity = &lora_data.pressure_velocity;
  acceleration = &lora_data.acceleration;
  gyro = &lora_data.gyro;
  latitude = &lora_data.latitude;
  longitude = &lora_data.longitude;
  gps_altitude = &lora_data.gps_altitude;
  heading = &lora_data.heading;
  pitch = &lora_data.pitch;
  roll = &lora_data.roll;

  pinMode(1, OUTPUT);
}

void recv_task(void *) {
  for (;;) {
    if ((len = Lora->Receive(&data[0], RECV_LEN)) > 0) {
      uint8_t *logger_ptr = data;

      memcpy(state, logger_ptr, sizeof(fsm_state_e));
      logger_ptr += sizeof(fsm_state_e);

      memcpy(systick, logger_ptr, sizeof(systick));
      logger_ptr += sizeof(systick);

      memcpy(pressure_altitude, logger_ptr, sizeof(pressure_altitude));
      logger_ptr += sizeof(pressure_altitude);

      memcpy(pressure_velocity, logger_ptr, sizeof(pressure_velocity));
      logger_ptr += sizeof(pressure_velocity);

      memcpy(longitude, logger_ptr, sizeof(longitude));
      logger_ptr += sizeof(longitude);

      memcpy(latitude, logger_ptr, sizeof(latitude));
      logger_ptr += sizeof(latitude);

      memcpy(gps_altitude, logger_ptr, sizeof(gps_altitude));
      logger_ptr += sizeof(gps_altitude);

      memcpy(acceleration, logger_ptr, sizeof(acceleration));
      logger_ptr += sizeof(acceleration);

      memcpy(gyro, logger_ptr, sizeof(gyro));
      logger_ptr += sizeof(gyro);

      memcpy(roll, logger_ptr, sizeof(roll));
      logger_ptr += sizeof(roll);

      memcpy(pitch, logger_ptr, sizeof(pitch));
      logger_ptr += sizeof(pitch);

      memcpy(heading, logger_ptr, sizeof(heading));
      logger_ptr += sizeof(heading);

      uint8_t ecc = 0;
      for (size_t i = 0; i < logger_ptr - &data[0]; i++) {
        ecc = ecc ^ data[i];
      }

      uint8_t tx_ecc;
      memcpy(&tx_ecc, logger_ptr, sizeof(tx_ecc));
      logger_ptr += sizeof(tx_ecc);

      if (tx_ecc == ecc) {
        recv_print();
        digitalWrite(1, led_status);
        led_status = !led_status;
      }
    }
  }
}

lora_data_t *lora_data_fetch() { return &lora_data; }

void recv_print() {
  printf("run_time: %d\n", *systick);
  printf("bmp280_altitude: %f\n", *pressure_altitude);
  printf("bmp280_velocity: %f\n", *pressure_velocity);
  printf("gps_longitude: %ld\n", *longitude);
  printf("gps_latitude: %ld\n", *latitude);
  printf("gps_altitude: %f\n", *gps_altitude);
  printf("imu_accel: x: %f, y: %f, z: %f\n", acceleration->x, acceleration->y, acceleration->z);
  printf("imu_gyro: x: %f, y: %f, z: %f\n", gyro->x, gyro->y, gyro->z);
  printf("imu_position: roll: %f, pitch: %f, heading: %f\n", *roll, *pitch, *heading);
  printf("----------------------------------------------\n");
}