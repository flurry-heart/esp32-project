#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "driver/spi_master.h"
#include "driver/gpio.h"

#define SPI_SCK_GPIO_NUM                GPIO_NUM_1
#define SPI_SCK(x)                      do{ x ? \
                                            gpio_set_level(SPI_SCK_GPIO_NUM, 1) : \
                                            gpio_set_level(SPI_SCK_GPIO_NUM, 0); \
                                        }while(0)

#define SPI_MISO_GPIO_NUM               GPIO_NUM_3
#define SPI_MISO()                      gpio_get_level(SPI_MISO_GPIO_NUM)

#define SPI_MOSI_GPIO_NUM               GPIO_NUM_2
#define SPI_MOSI(x)                     do{ x ? \
                                            gpio_set_level(SPI_MOSI_GPIO_NUM, 1) : \
                                            gpio_set_level(SPI_MOSI_GPIO_NUM, 0); \
                                        }while(0)

#define SIMULATE_SPI_MODE0              0
#define SIMULATE_SPI_MODE1              1
#define SIMULATE_SPI_MODE2              2
#define SIMULATE_SPI_MODE3              3

#define SIMULATE_SPI_MODE               SIMULATE_SPI_MODE0

void bsp_spi_init(spi_host_device_t host_id, gpio_num_t spi_sclk_io_num, gpio_num_t spi_miso_io_num, gpio_num_t spi_mosi_io_num);
void bsp_spi_bus_add_device(spi_device_handle_t *device_handle, spi_host_device_t host_id, gpio_num_t cs_gpio_num, uint8_t mode, int clock_speed);
void bsp_spi_send_one_byte(spi_device_handle_t device_handle, uint8_t data);
void bsp_spi_send_bytes(spi_device_handle_t device_handle, const uint8_t *data, uint16_t length);
uint8_t bsp_spi_transfer_one_byte(spi_device_handle_t device_handle, uint8_t data);

void bsp_simulate_spi_init(void);
uint8_t bsp_simulate_spi_swap_one_byte(uint8_t data);

#endif // !__BSP_SPI_H__