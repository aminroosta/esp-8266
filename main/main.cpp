#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "esp_aio.h"
#include "esp_interface.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define px(a) printf("%s => %#010x\n", (#a), (a))
#define pd(a) printf("%s => %d\n", (#a), (a))
#define ps(a) printf("%s => %s\n", (#a), (a))
#define rep(i, n) for (int i = 0; i < n; ++i)

#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_NUM_2)


extern "C" void app_main()
{
     
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = BIT(GPIO_NUM_2);
    gpio_config(&io_conf);

    ps(esp_get_idf_version());

    uint32_t enabled = 1;
    for (int i = 100; i >= 0; i--)
    {
        printf(".");
        fflush(stdout);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_2, enabled);
        enabled = !enabled;
    }
    printf("\nRestarting now.\n");
    fflush(stdout);
    esp_restart();
}