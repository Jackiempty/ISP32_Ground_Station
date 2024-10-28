#ifndef _INFLUXDB_
#define _INFLUXDB_

// Wifi: 0, AP: 1
#define STATUS 0

#include <WiFi.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFiMulti.h>
#include "ra01s.h"
#include "recv.h"

void influxdb_init();
void influxdb_task();

#endif