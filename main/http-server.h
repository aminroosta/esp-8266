#include <vector>
#include <esp_http_server.h>

struct http_server_helper;
struct http_server {
    http_server(http_server const&) = delete;
    http_server(http_server &&) = delete;

    http_server();

    void connect (char const *ssid, char const *password);

    void add_route(httpd_uri_t route);

    friend struct http_server_helper;
private:
    void stop();
    void start(); 

    void* server;
    bool is_started;
    std::vector<httpd_uri_t> routes;
};