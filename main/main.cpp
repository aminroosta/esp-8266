#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "debug.h"
#include "esp_http_server.h"
#include "http-server.h"

esp_err_t hello_get_handler(httpd_req_t *req)
{
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        auto host = (char *) malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", host, buf_len) == ESP_OK) {
            dbg(host);
        }
        free(host);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header") + 1;
    if (buf_len > 1) {
        auto test_header = (char*) malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header", test_header, buf_len) == ESP_OK) {
            dbg(test_header);
        }
        free(test_header);
    }

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        auto buf = (char*) malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            dbg(buf);
            char param[32];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
                dbg(param);
            }
            if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
                dbg(param);
            }
            if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
                dbg(param);
            }
        }
        free(buf);
    }

    /* Set some custom headers */
    httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    /* After sending the HTTP response the old HTTP request */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        puts("Request headers lost");
    }
    return ESP_OK;
}

void app() {
    if (nvs_flash_init() == ESP_ERR_NVS_NO_FREE_PAGES) {
        dbg(nvs_flash_erase());
        dbg(nvs_flash_init());
    }
    puts("started");

    static http_server server{};

    server.add_route({"/hello",
                      HTTP_GET,
                      hello_get_handler,
                      (void *)"Hello World!"});

    server.connect("**", "************");

    for (;;) {
        vTaskDelay(1000);
    }
    printf("\nRestarting now.\n"); fflush(stdout);
    esp_restart();
}

extern "C" void app_main() { app(); }