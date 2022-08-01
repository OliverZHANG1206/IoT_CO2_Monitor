#ifndef NETWORK_TIME_H
#define NETWORK_TIME_H

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_sntp.h"

void network_time_init(void);
void network_time(void);
void time_adj(void);
char* get_time(void);

#endif /* NETWORK_TIME_H*/