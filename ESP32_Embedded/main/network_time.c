#include "network_time.h"

static const char *TAG = "TIME SERVICE";

/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
//static void obtain_time(void);
//static void initialize_sntp(void);

time_t now = 0;
char strftime_buf[64];
struct tm timeinfo = { 0 };

void network_time_init(void)
{
    // Set timezone to Eastern Standard Time and print local time
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();

    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();

    sntp_servermode_dhcp(1);
    
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
    sntp_init();
    ESP_LOGI(TAG, "Initialized Successful");
    
    ESP_LOGI(TAG, "Update time...");
    network_time();
    time_adj();
}

void network_time(void)
{
    int retry = 0;
    const int retry_count = 10;

    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) 
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    time(&now);
    
    if (retry == retry_count) ESP_LOGI(TAG, "Update Failed");
    else ESP_LOGI(TAG, "Update Successful");
}

void time_adj(void)
{
    struct timeval outdelta;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS) 
    {
        adjtime(NULL, &outdelta);
        ESP_LOGI(TAG, "Waiting for adjusting time ... outdelta = %li sec: %li ms: %li us",
                    (long)outdelta.tv_sec,
                    outdelta.tv_usec/1000,
                    outdelta.tv_usec%1000);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

char* get_time(void)
{
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    return strftime_buf;
}