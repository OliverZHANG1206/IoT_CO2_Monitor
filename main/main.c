#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
//#include "driver/gpio.h"

#include "scd30.h"
#include "am312.h"
#include "wifi.h"
#include "mqtt_client.h"
#include "mqtt_network.h"
#include "network_time.h"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void)
{
	//static float res[3] = {};
	wifi_init();
	mqtt_init();
	network_time_init();
	
	for(int i = 0; i < 10; i++)
	{
		//mqtt_publish("Second");
		printf("Time: %s\n", get_time());
		sleep(2);
	}
	//scd30_init();
	//scd30_info();
	//am312_init();
	//mqtt_init();
	//scd30_start_period_measurement(0);
	//vTaskDelay(100);
	//scd30_stop_period_measurement();
	//vTaskDelay(1000);
	//scd30_set_measurement_interval(2);
	//vTaskDelay(100);
	//scd30_start_period_measurement(0);

	//scd30_get_measurement_interval();
	//scd30_read_measurement();

	/*
	while (1)
	{
		//printf("%d\n", gpio_get_level(27));
		if (scd30_get_data_ready())
		{
			scd30_read_measurement(res);
			printf("CO2: %.3fppm;  Temperature: %.2fC;  humidity: %.2f%%\n", res[0], res[1], res[2]);
		}
		vTaskDelay(5);
	}
	*/

//    nvs_flash_init();
//    tcpip_adapter_init();
//    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
//    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // @suppress("Type cannot be resolved")
//    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
//    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
//    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
//    wifi_config_t sta_config = {
//        .sta = {
//            .ssid = CONFIG_ESP_WIFI_SSID,
//            .password = CONFIG_ESP_WIFI_PASSWORD,
//            .bssid_set = false
//        }
//    };
//    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
//    ESP_ERROR_CHECK( esp_wifi_start() );
//    ESP_ERROR_CHECK( esp_wifi_connect() );
//
//    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
//    int level = 0;
//    while (true) {
//        gpio_set_level(GPIO_NUM_4, level);
//        level = !level;
//        vTaskDelay(300 / portTICK_PERIOD_MS);
//    }
}

