#include "mqtt_network.h"

static const char *TAG = "MQTT";
struct mqtt_topic command_receive = {"/Command/Sensor0", 0};
struct mqtt_topic data_publish    = {"/Sensor_data/Sensor0", 0};

static EventGroupHandle_t mqtt_event_group;
static esp_mqtt_client_handle_t client;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base = %s, event_id = %d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    //esp_mqtt_client_handle_t client = event->client;

	switch ((esp_mqtt_event_id_t)event_id) 
	{
	    case MQTT_EVENT_CONNECTED:
	        ESP_LOGI(TAG, "MQTT CONNECTED");
			command_receive.msg_id = esp_mqtt_client_subscribe(client, command_receive.topic, 0);
			ESP_LOGI(TAG, "Sent subscribe successful, msg_id = %d", command_receive.msg_id);

	        data_publish.msg_id = esp_mqtt_client_subscribe(client, data_publish.topic, 0);
	        ESP_LOGI(TAG, "Sent subscribe successful, msg_id = %d", data_publish.msg_id);
	        break;

	    case MQTT_EVENT_DISCONNECTED:
	        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
	        break;

	    case MQTT_EVENT_SUBSCRIBED:
			if (event->msg_id == command_receive.msg_id)
			{
				ESP_LOGI(TAG, "MQTT SUBSCRIBED TOPIC \"%s\", msg_id=%d", command_receive.topic, event->msg_id);
				xEventGroupSetBits(mqtt_event_group, MQTT_CONNECTED_COMMAND_BIT);
			}
			if (event->msg_id == data_publish.msg_id)
			{
				ESP_LOGI(TAG, "MQTT SUBSCRIBED TOPIC \"%s\", msg_id=%d", data_publish.topic, event->msg_id);
				mqtt_publish("Connect successfully with Sensor");
				xEventGroupSetBits(mqtt_event_group, MQTT_CONNECTED_DATA_BIT);
			}
	        break;

	    case MQTT_EVENT_UNSUBSCRIBED:
	        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
	        break;

	    case MQTT_EVENT_PUBLISHED:
	        ESP_LOGI(TAG, "MQTT PUBLISHED ON TOPIC \"%s\", msg_id = %d", event->topic, event->msg_id);
	        break;

	    case MQTT_EVENT_DATA:
	        ESP_LOGI(TAG, "DATA: \r\nTOPIC = %.*s\r\nDATA = %.*s\r\n", event->topic_len, event->topic, event->data_len, event->data);
	        //printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
	        //printf("DATA=%.*s\r\n", event->data_len, event->data);
	        break;

	    case MQTT_EVENT_ERROR:
	        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
	        break;

	    default:
	        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
	        break;
	}
}

void mqtt_init(void)
{
	const esp_mqtt_client_config_t mqtt_cfg =
	{
	    .uri = BROKER_URL,
		.port = BROKER_PORT,
	};

	mqtt_event_group = xEventGroupCreate();

	client = esp_mqtt_client_init(&mqtt_cfg);
	esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
	esp_mqtt_client_start(client);

	EventBits_t bits = xEventGroupWaitBits(mqtt_event_group, MQTT_CONNECTED_COMMAND_BIT | MQTT_CONNECTED_DATA_BIT, pdFALSE, pdTRUE, pdMS_TO_TICKS(5000));

	if (bits) ESP_LOGI(TAG, "MQTT Initialized Successfully");
	else ESP_LOGI(TAG, "Failed to Initialize MQTT");

	vEventGroupDelete(mqtt_event_group);
}

void mqtt_publish(char* data)
{
	int msg = esp_mqtt_client_publish(client, data_publish.topic, data, 0, 0, 0);

	if (msg == -1) ESP_LOGI(TAG, "Failed to publish");
	else ESP_LOGI(TAG, "Sent publish successful");
}

