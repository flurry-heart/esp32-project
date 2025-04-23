#ifndef __BSP_EXIT_H__
#define __BSP_EXIT_H__

#include "driver/gpio.h"
#include "esp_system.h"

void bsp_exit_init(gpio_num_t gpio_num, uint8_t trigger_edge);

#endif // !__BSP_EXIT_H__
