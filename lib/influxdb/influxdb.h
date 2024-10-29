#ifndef _INFLUXDB_
#define _INFLUXDB_

#define MODE AP
// #define MODE STA

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <string>

#include "ra01s.h"
#include "recv.h"

void influxdb_init();
void influxdb_task();

#endif