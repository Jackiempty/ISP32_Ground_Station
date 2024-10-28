#include "recv.h"

#include <stddef.h>
#include <stdint.h>

#define TAG "RECV"
static lora_data_t lora_data;

#define RECV_LEN 255

void recv_task(SX126x *lora) {
  uint8_t data[RECV_LEN];
  size_t len;
  uint8_t led_status = 0;

  fsm_state_e *state = &lora_data.state;
  uint32_t *systick = &lora_data.systick;
  float *pressure_altitude = &lora_data.pressure_altitude;
  float *pressure_velocity = &lora_data.pressure_velocity;
  vector_t *acceleration = &lora_data.acceleration;
  vector_t *gyro = &lora_data.gyro;
  int32_t *latitude = &lora_data.latitude;
  int32_t *longitude = &lora_data.longitude;
  float *gps_altitude = &lora_data.gps_altitude;
  float *heading = &lora_data.heading;
  float *pitch = &lora_data.pitch;
  float *roll = &lora_data.roll;

  pinMode(1, OUTPUT);

  for (;;) {
    if ((len = lora->Receive(&data[0], RECV_LEN)) > 0) {
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
        printf(">>>%u, %lu, %f, %f, %ld, %ld, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n<<<\n", *state, *systick, *pressure_altitude,
               *pressure_velocity, *longitude, *latitude, *gps_altitude, acceleration->x, acceleration->y, acceleration->z, gyro->x,
               gyro->y, gyro->z, *roll, *pitch, *heading);

        digitalWrite(1, led_status);
        led_status = !led_status;
      }
    }
  }
}

lora_data_t *lora_data_fetch() { return &lora_data; }

void recv_print() {
  printf("bmp280_altitude: %f\n", lora_data.pressure_altitude);
  printf("bmp280_velocity: %f\n", lora_data.pressure_velocity);
  printf("gps_longitude: %ld\n", lora_data.longitude);
  printf("gps_latitude: %ld\n", lora_data.latitude);
  printf("gps_altitude: %f\n", lora_data.gps_altitude);
  printf("imu_accel: x: %f, y: %f, z: %f\n", lora_data.acceleration.x, lora_data.acceleration.y, lora_data.acceleration.z);
  printf("imu_gyro: x: %f, y: %f, z: %f\n", lora_data.gyro.x, lora_data.gyro.y, lora_data.gyro.z);
  printf("imu_position: roll: %f, pitch: %f, heading: %f\n", lora_data.roll, lora_data.pitch, lora_data.heading);
  printf("----------------------------------------------\n");
}