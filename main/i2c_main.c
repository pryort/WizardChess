// i2c_main.c (legacy/old API)
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_PORT_SHARED       I2C_NUM_0   // reuse same port as GT911
#define I2C_SHARED_SDA_IO     8           // do not reconfigure
#define I2C_SHARED_SCL_IO     9

static const char *TAG = "i2c_shared";

// Do NOT install or reconfigure the driver for shared bus
esp_err_t i2c_comm_init(void)
{
    ESP_LOGI(TAG, "Reusing I2C port %d (pins already owned by display/touch).", I2C_PORT_SHARED);
    return ESP_OK;
}

esp_err_t i2c_comm_write(uint8_t addr, const uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data, len, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT_SHARED, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t i2c_comm_read(uint8_t addr, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, true);
    if (len > 1) i2c_master_read(cmd, data, len - 1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, data + len - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT_SHARED, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}
