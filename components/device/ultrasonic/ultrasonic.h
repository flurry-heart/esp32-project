#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include "driver/gpio.h"
#include "esp_rom_sys.h"

#include "bsp_mcpwm.h"

void ultrasonic_init(gpio_num_t trig_gpio_num);
void ultrasonic_trigger(gpio_num_t trig_gpio_num);

#endif // !__HR_SR04_H__