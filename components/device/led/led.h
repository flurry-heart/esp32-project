#ifndef __LED_H__
#define __LED_H__

#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"

void led_init(gpio_num_t gpio_num);
void led_set_status(gpio_num_t gpio_num, uint8_t status);
void led_toggle_status(gpio_num_t gpio_num);

#endif // !__LED_H__