#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_SSID           "LAPTOP-700UPD1B 3454"
#define WIFI_PASSWORD       "1415926zyfA"
#define WIFI_MAXIMUM_RETRY  5
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

void wifi_init(void);
void wifi_connect(void);

#endif /* MAIN_WIFI_H_ */
