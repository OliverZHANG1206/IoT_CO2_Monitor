#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wifi.h"
#include "scd30.h"
#include "am312.h"
#include "mqtt_client.h"
#include "mqtt_network.h"
#include "network_time.h"

static float result[3] = {};
static char data_stream[80] = "";

void app_main(void)
{
	wifi_init();
	mqtt_init();
	am312_init();
	scd30_init();
	network_time_init();
	vTaskDelay(2000 / portTICK_PERIOD_MS);

	scd30_set_measurement_interval(10);
	vTaskDelay(2000 / portTICK_PERIOD_MS);

	for(;;)
	{
		if (scd30_get_data_ready())
		{
			scd30_read_measurement(result);
			sprintf(data_stream, "[%s] SCD30:[%.3f; %.3f; %.3f] AM312:[%d]", get_time(), result[0], result[1], result[2], get_trigger_num());
			mqtt_publish(data_stream);
		}
		vTaskDelay(30000 / portTICK_PERIOD_MS);	
	}
}

