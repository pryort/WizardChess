/*I2C Data translation
    Initial start up sends A1 B2 C3 to slave

    After user selects piece movement and color the data is sent in the form:
    3D ##
    where 3D is the preamble, and the first number:
    1: physical piece movement
    2: user interface
    3: online
    the second number:
    1: black
    2: white

    When a piece is selected the data is sent in the form:
    AA ##
    where AA is the preamble
    first number indicate the column
    second number indicates the row
    following this form:
    00 10 20 30 40 50 60 70
    01 11 21 31 41 51 61 71
    02 12 22 32 42 52 62 72
    03 13 23 33 43 53 63 73
    04 14 24 34 44 54 64 74
    05 15 25 35 45 55 65 75
    06 16 26 36 46 56 66 76
    07 17 27 37 47 57 67 77

    When a piece is moved, the last selected piece is the one moved, and the data is sent in the form:
    FF ##
    where FF is the preamble and the location follows the same form as for the selected piece
    */
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
