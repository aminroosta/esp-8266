#include <vector>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_http_server.h>
#include "debug.h"
#include "http-server.h"

struct http_server_helper
{
    static esp_err_t event_handler(void *ctx, system_event_t *event)
    {
        auto server = (http_server *)ctx;
        /* For accessing reason codes in case of disconnection */
        system_event_info_t *info = &event->event_info;

        switch (event->event_id)
        {
        case SYSTEM_EVENT_STA_START:
            dbg(esp_wifi_connect());
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            dbg(ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

            server->start();
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            dbg(info->disconnected.reason);
            if (info->disconnected.reason == WIFI_REASON_BASIC_RATE_NOT_SUPPORT)
            {
                /*Switch to 802.11 bgn mode */
                esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N);
            }
            dbg(esp_wifi_connect());

            server->stop();
            break;
        default:
            break;
        }
        return ESP_OK;
    }
};

http_server::http_server() : server(nullptr), is_started(false){};

void http_server::add_route(httpd_uri_t route)
{
    routes.push_back(route);
}

void http_server::connect(char const *ssid, char const *password)
{
    tcpip_adapter_init();

    dbg(esp_event_loop_init(http_server_helper::event_handler, this));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    dbg(esp_wifi_init(&cfg));
    dbg(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    wifi_config_t config = {};
    strcpy((char *)config.sta.ssid, ssid);
    strcpy((char *)config.sta.password, password);

    dbg(esp_wifi_set_mode(WIFI_MODE_STA));
    dbg(esp_wifi_set_config(ESP_IF_WIFI_STA, &config));

    dbg(esp_wifi_start());
}

void http_server::stop()
{
    if (is_started)
    {
        is_started = false;
        httpd_stop(server);
        server = nullptr;
    }
}

void http_server::start(void)
{
    if (is_started)
    {
        return;
    }

    server = nullptr;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    dbg(config.server_port);

    if (dbg(httpd_start(&server, &config)) == ESP_OK)
    {
        for (auto &r : routes)
        {
            httpd_register_uri_handler(server, &r);
        }
        is_started = true;
    }
    dbg(is_started);
}
