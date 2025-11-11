#include "waveshare_rgb_lcd_port.h"
#include "user_gameplay.h"
#include "i2c_main.h"


void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init(); // Initialize the Waveshare ESP32-S3 RGB LCD 

    ESP_ERROR_CHECK(i2c_comm_init());

    uint8_t tx_data[] = {0xA1, 0xB2, 0xC3};
    i2c_comm_write(0x67, tx_data, sizeof(tx_data));

    vTaskDelay(pdMS_TO_TICKS(10));

    uint8_t rx_data[3] = {0};
    i2c_comm_read(0x67, rx_data, sizeof(rx_data));
    printf("%X %X %X\n", rx_data[0], rx_data[1], rx_data[2]);

    // Lock the mutex
    if (lvgl_port_lock(-1)) {
        create_starting_menu();
        lvgl_port_unlock();
    }
}
