#ifndef __ROTARY_ENCODER_H__
#define __ROTARY_ENCODER_H__

#include "driver/gpio.h"

void rotary_encoder_init(gpio_num_t clk_pin, gpio_num_t dt_pin, gpio_num_t sw_pin);
uint8_t rotary_encoder_scan(gpio_num_t clk_pin, gpio_num_t dt_pin, gpio_num_t sw_pin);

#endif // !__ROTARY_ENCODER_H__