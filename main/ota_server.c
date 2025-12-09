#include "ota_server.h"
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/ringbuf.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"

static const char *TAG = "OTA";

// --- Configuration ---
#define WIFI_SSID      "WizardChess_Display"
#define WIFI_PASS      "Wizrd"
#define LOG_BUFFER_SIZE (4 * 1024)

static RingbufHandle_t log_ringbuf = NULL;
static httpd_handle_t server = NULL;

// --- HTML Content ---
static const char *upload_page = 
    "<!DOCTYPE html><html><body style='font-family: sans-serif;'>"
    "<h1>Wizard Chess Board</h1>"
    "<h2>Firmware Update</h2>"
    "<form method='POST' action='/update' enctype='multipart/form-data'>"
    "<input type='file' name='update'><br><br>"
    "<input type='submit' value='Upload & Flash'>"
    "</form>"
    "<br><hr><br>"
    "<h2><a href='/log'>View Live Logs</a></h2>"
    "</body></html>";

static const char *log_page = 
    "<!DOCTYPE html><html><head><title>Board Logs</title>"
    "<style>"
    "body { background: #222; color: #0f0; font-family: monospace; padding: 10px; }"
    "#logs { white-space: pre-wrap; word-wrap: break-word; }"
    "</style></head><body>"
    "<h3>Connecting to WebSocket...</h3>"
    "<div id='logs'></div>"
    "<script>"
    "var ws = new WebSocket('ws://' + location.host + '/ws');"
    "ws.onopen = function() { document.querySelector('h3').innerText = 'Log Stream Connected'; };"
    "ws.onmessage = function(event) {"
    "  var d = document.getElementById('logs');"
    "  d.innerHTML += event.data;"
    "  window.scrollTo(0, document.body.scrollHeight);"
    "};"
    "</script></body></html>";

// --- Custom Log Handler ---
// This intercepts ESP_LOG calls and sends them to the Ring Buffer
int web_log_vprintf(const char *fmt, va_list args) {
    // 1. Copy args because vsnprintf consumes them
    va_list args_copy;
    va_copy(args_copy, args);
    
    // 2. Format message into buffer
    char buf[256];
    int len = vsnprintf(buf, sizeof(buf), fmt, args_copy);
    va_end(args_copy);

    // 3. Send to RingBuffer (if connected)
    if (log_ringbuf && len > 0) {
        // Send, allow dropping if full (0 wait) to prevent stalling the main CPU
        xRingbufferSend(log_ringbuf, buf, len, 0);
    }

    // 4. Print to standard Serial as well
    return vprintf(fmt, args);
}

// --- WebSocket Handler ---
static esp_err_t ws_handler(httpd_req_t *req) {
    if (req->method == HTTP_GET) return ESP_OK; // Handshake

    // Keep connection alive by reading (discarding) input
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    uint8_t buf[16];
    ws_pkt.payload = buf;
    httpd_ws_recv_frame(req, &ws_pkt, sizeof(buf));
    return ESP_OK;
}

// --- Log Pusher Task ---
// Reads from RingBuffer and sends to all connected WebSocket clients
static void log_pusher_task(void *param) {
    char *item;
    size_t item_size;
    while (1) {
        item = (char *)xRingbufferReceive(log_ringbuf, &item_size, pdMS_TO_TICKS(200));
        if (item != NULL) {
            size_t fds = 4;
            int client_fds[4];
            // Get list of active clients
            if (httpd_get_client_list(server, &fds, client_fds) == ESP_OK) {
                for (int i = 0; i < fds; i++) {
                    // Check if client is WebSocket
                    if (httpd_ws_get_fd_info(server, client_fds[i]) == HTTPD_WS_CLIENT_WEBSOCKET) {
                        httpd_ws_frame_t ws_pkt = {
                            .type = HTTPD_WS_TYPE_TEXT,
                            .payload = (uint8_t *)item,
                            .len = item_size
                        };
                        httpd_ws_send_frame_async(server, client_fds[i], &ws_pkt);
                    }
                }
            }
            vRingbufferReturnItem(log_ringbuf, (void *)item);
        }
    }
}

// --- URI Handlers ---
static esp_err_t index_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, upload_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t log_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, log_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t update_post_handler(httpd_req_t *req) {
    char buf[1024];
    esp_ota_handle_t update_handle = 0 ;
    const esp_partition_t *update_partition = NULL;
    int received;
    int remaining = req->content_len;
    bool image_header_checked = false;

    update_partition = esp_ota_get_next_update_partition(NULL);
    if (update_partition == NULL) {
        ESP_LOGE(TAG, "Partition error");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    while (remaining > 0) {
        if ((received = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
            if (received == HTTPD_SOCK_ERR_TIMEOUT) continue;
            esp_ota_end(update_handle);
            return ESP_FAIL;
        }

        if (!image_header_checked) {
            esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
            image_header_checked = true;
        }
        esp_ota_write(update_handle, buf, received);
        remaining -= received;
    }

    if (esp_ota_end(update_handle) == ESP_OK) {
        esp_ota_set_boot_partition(update_partition);
        httpd_resp_sendstr(req, "Update Success! Rebooting...");
        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_restart();
    } else {
        httpd_resp_send_500(req);
    }
    return ESP_OK;
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "Client connected: " MACSTR, MAC2STR(event->mac));
    }
}

void start_ota_server(void) {
    // 1. Setup Wifi AP
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .channel = 1,
            .password = WIFI_PASS,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(WIFI_PASS) == 0) wifi_config.ap.authmode = WIFI_AUTH_OPEN;

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    // 2. Setup Logging redirection
    log_ringbuf = xRingbufferCreate(LOG_BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);
    xTaskCreate(log_pusher_task, "log_push", 4096, NULL, 5, NULL);
    esp_log_set_vprintf(web_log_vprintf); // <-- This captures ESP_LOGI calls

    // 3. Start Web Server
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/", .method=HTTP_GET, .handler=index_get_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/log", .method=HTTP_GET, .handler=log_get_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/ws", .method=HTTP_GET, .handler=ws_handler, .is_websocket=true});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/update", .method=HTTP_POST, .handler=update_post_handler});
        
        ESP_LOGI(TAG, "OTA Server Started. Connect to '%s' and go to 192.168.4.1", WIFI_SSID);
    }
}