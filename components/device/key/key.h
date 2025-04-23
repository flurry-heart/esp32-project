#ifndef __KEY_H__
#define __KEY_H__

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"

void key_init(gpio_num_t gpio_num, uint8_t press_level);
bool key_scan(gpio_num_t gpio_num, uint8_t press_level);

#endif // !__KEY_H__