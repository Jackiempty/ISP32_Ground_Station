#ifndef _INFLUXDB_
#define _INFLUXDB_

#include <WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

void influxdb_init();
void influxdb_task();

#endif