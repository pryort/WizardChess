#include "waveshare_rgb_lcd_port.h"
#include "user_gameplay.h"
#include "i2c_main.h"
#include "ota_server.h"
#include "nvs_flash.h"


void app_main()
{

    // Initialize NVS (Required for WiFi)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Start the Hotspot + Log Server
    start_ota_server();
    
    waveshare_esp32_s3_rgb_lcd_init(); // Initialize the Waveshare ESP32-S3 RGB LCD 

    ESP_ERROR_CHECK(i2c_comm_init());

    vTaskDelay(pdMS_TO_TICKS(1000));

    uint8_t tx_data[] = {0xA1, 0xB2, 0xC3};
    i2c_comm_write(0x67, tx_data, sizeof(tx_data));

    

    uint8_t rx_data[3] = {0};
    i2c_comm_read(0x67, rx_data, sizeof(rx_data));
    printf("%X %X %X\n", rx_data[0], rx_data[1], rx_data[2]);
    while(1) {
        if(rx_data[0] != 0x1A && rx_data[1] != 0x2B && rx_data[2] != 0x3C) {
            i2c_comm_write(0x67, tx_data, sizeof(tx_data));
            vTaskDelay(pdMS_TO_TICKS(1000));
            i2c_comm_read(0x67, rx_data, sizeof(rx_data));
            printf("%X %X %X\n", rx_data[0], rx_data[1], rx_data[2]);
        }
        else {
            break;
        }
    }   

    // Lock the mutex
    if (lvgl_port_lock(-1)) {
        create_starting_menu();
        lvgl_port_unlock();
    }
}
