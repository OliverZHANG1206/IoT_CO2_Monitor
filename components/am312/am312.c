#include "am312.h"

uint8_t led_state = 0;
struct timeval start_t, end_t;
static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    uint16_t gpio_val = gpio_get_level(AM312_GPIO);
    float    interval = 0;

    if (gpio_num != AM312_GPIO) return;

    led_state = !led_state;
    gpio_set_level(2, led_state);

    if (gpio_val) gettimeofday(&start_t, NULL);
    else
    {
    	gettimeofday(&end_t, NULL);
    	interval = (end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec) / 1e6 - DEAD_TIME;
    	xQueueSendFromISR(gpio_evt_queue, &interval, NULL);
    }
}

static void print_task(void* arg)
{
    float interval = 0;
    static uint16_t times = 0;

    for(;;)
    {
        if(xQueueReceive(gpio_evt_queue, &interval, portMAX_DELAY))
        {
        	times++;
            printf("Trigger: %d; Interval: %.3fs\n", times, interval);
        }
    }

}

void am312_init(void)
{
	gpio_config_t io_conf =
	{
		.mode = GPIO_MODE_INPUT,
		.intr_type = GPIO_INTR_ANYEDGE,
		.pin_bit_mask = (1ULL << AM312_GPIO),
		.pull_up_en = ENABLE,
		.pull_down_en = DISABLE,
	};
	esp_err_t res1 = gpio_config(&io_conf);
	esp_err_t res2 = gpio_install_isr_service(0);
	esp_err_t res3 = gpio_isr_handler_add(AM312_GPIO, gpio_isr_handler, (void*) AM312_GPIO);
	esp_err_t res4 = gpio_intr_enable(AM312_GPIO);

	gpio_evt_queue = xQueueCreate(10, sizeof(float));
	xTaskCreate(print_task, "print_task", 2048, NULL, 10, NULL);

	gpio_set_direction(2, GPIO_MODE_OUTPUT);
	gpio_set_level(2, led_state);

	if (res1 == ESP_OK && res2 == ESP_OK && res3 == ESP_OK && res4 == ESP_OK) printf("Set AM312 successfully\n");
	else printf("Failed to set AM312.\n");
}
