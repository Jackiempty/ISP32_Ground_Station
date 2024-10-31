#ifndef _INFLUXDB_
#define _INFLUXDB_

#define MODE 1  //   0: AP, 1: STA

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include <string>

#include "ra01s.h"
#include "recv.h"

void influxdb_init();
void influxdb_task(void *);

#endif