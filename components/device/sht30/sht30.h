#ifndef __SHT30_H__
#define __SHT30_H__

#include "driver/i2c.h"

#include "bsp_i2c.h"

// #define SH30_USE_HARDWARE_I2C

#define SHT30_I2C_DEVICE_ADDRESS        0x44

#ifdef SH30_USE_HARDWARE_I2C

extern i2c_master_dev_handle_t g_sht30_i2c_device_handle;

#endif // SH30_USE_HARDWARE_I2C

void sht30_set_periodic_read_mode(uint16_t read_cmd);
void sht30_read_data(uint16_t read_cmd, float *temperature, float *humidity);

#endif // !__SHT30_H__