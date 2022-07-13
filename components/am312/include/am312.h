#ifndef AM312_H
#define AM312_H

#include <stdio.h>
#include <time.h>
#include "sys/time.h"
#include "sys/unistd.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define AM312_GPIO   27
#define ENABLE       1
#define DISABLE      0
#define DEAD_TIME    2.3

void am312_init(void);

#endif /* AM312_H */
