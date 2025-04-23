#ifndef __BSP_LEDC_H__
#define __BSP_LEDC_H__

#include "driver/ledc.h"

void bsp_ledc_init(ledc_timer_t time_num, ledc_channel_t ledc_channel, int gpio_num, ledc_timer_bit_t resolution, uint32_t freq, uint32_t duty);
void bsp_ledc_set_pwm_duty(ledc_channel_t ledc_channel, uint32_t duty);
void bsp_ledc_set_gradient_range(ledc_channel_t ledc_channel, uint32_t start_value, uint32_t end_value, int gradient_time);

#endif // !__BSP_LEDC_H__