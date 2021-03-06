#include "wifi.h"

static const char *TAG = "WIFI";

static int retry_num = 0;
static EventGroupHandle_t wifi_event_group;
esp_event_handler_instance_t instance_any_id;
esp_event_handler_instance_t instance_got_ip;

static void nvs_flash_check(void)
{
	esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
	{
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
	{
		esp_wifi_connect();
	}
	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
	{
        if (retry_num < WIFI_MAXIMUM_RETRY) 
		{
            esp_wifi_connect();
            retry_num++;
            ESP_LOGI(TAG, "Retry to connect to the AP");
        } 
		else xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
        ESP_LOGI(TAG,"Connect to the AP failed");
    } 
	else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
	{
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        retry_num = 0;
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init(void)
{
	wifi_event_group = xEventGroupCreate();

	nvs_flash_check();
	esp_netif_init();
	esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);

	esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip);

	wifi_config_t wifi_config =
	{
	    .sta =
	    {
	        .ssid = WIFI_SSID,
	        .password = WIFI_PASSWORD,
		    .threshold.authmode = WIFI_AUTH_WPA2_PSK,
	    },
	};
	esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
	ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_start());

	ESP_LOGI(TAG, "WIFI_INIT_FIN");

	wifi_connect();
}

void wifi_connect()
{
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) ESP_LOGI(TAG, "Connected to AP SSID:%s password:%s", WIFI_SSID, WIFI_PASSWORD); 
	else if (bits & WIFI_FAIL_BIT) ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
    else                           ESP_LOGE(TAG, "UNEXPECTED EVENT");

    esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip);
    esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id);
    vEventGroupDelete(wifi_event_group);
}
