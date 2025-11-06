#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t i2c_comm_init(void);
esp_err_t i2c_comm_write(uint8_t addr, const uint8_t *data, size_t len);
esp_err_t i2c_comm_read(uint8_t addr, uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif
