/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "waveshare_rgb_lcd_port.h"
#include "user_gameplay.h"


void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init(); // Initialize the Waveshare ESP32-S3 RGB LCD 
    ESP_LOGI(TAG, "Display LVGL demos");
    // Lock the mutex
    if (lvgl_port_lock(-1)) {
        create_starting_menu();
        lvgl_port_unlock();
    }
}
