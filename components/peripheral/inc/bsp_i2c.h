#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include "driver/i2c_master.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"

#include "esp_rom_sys.h"

#define I2C_SCL_GPIO_NUM            GPIO_NUM_1
#define I2C_SDA_GPIO_NUM            GPIO_NUM_2

#define I2C_SCL(x)                  gpio_set_level(I2C_SCL_GPIO_NUM, x)
#define I2C_SDA(x)                  gpio_set_level(I2C_SDA_GPIO_NUM, x)

#define I2C_READ_SDA()              gpio_get_level(I2C_SDA_GPIO_NUM)

// #define I2C_DELAY()                 vTaskDelay(pdMS_TO_TICKS(1))
#define I2C_DELAY()                 esp_rom_delay_us(10)

extern i2c_master_bus_handle_t g_i2c_master_bus_handle;

void bsp_i2c_init(i2c_master_bus_handle_t *handle, i2c_port_t i2c_num, gpio_num_t scl_gpio_num, gpio_num_t sda_gpio_num);
void bsp_i2c_bus_add_device(i2c_master_dev_handle_t *device_handle, i2c_master_bus_handle_t bus_handle, uint16_t device_address, i2c_addr_bit_len_t device_address_length, int clock_speed, uint32_t scl_wait_us);
void bsp_i2c_send_one_byte(i2c_master_dev_handle_t device_handle, uint8_t data);
void bsp_i2c_send_bytes(i2c_master_dev_handle_t device_handle, const uint8_t *data, uint16_t length);
uint8_t bsp_i2c_receive_one_byte(i2c_master_dev_handle_t device_handle);
void bsp_i2c_receive_bytes(i2c_master_dev_handle_t device_handle, uint8_t *data, uint16_t length);
void bsp_i2c_transmit_receive(i2c_master_dev_handle_t device_handle, uint8_t *write_buffer, size_t write_size, uint8_t *read_buffer, size_t read_size);

void bsp_simulate_i2c_init(void);
void bsp_simulate_i2c_start(void);
void bsp_simulate_i2c_stop(void);
uint8_t bsp_simulate_i2c_wait_ack(void);
void bsp_simulate_i2c_send_ack(uint8_t ack);
uint8_t bsp_simulate_i2c_read_one_byte(uint8_t ack);
void bsp_simulate_i2c_send_one_byte(uint8_t data);

#endif // !__BSP_I2C_H__