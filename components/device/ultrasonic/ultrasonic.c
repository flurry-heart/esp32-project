#include "ultrasonic.h"

/**
 * @brief 初始化超声波模块
 * 
 * @param trig_gpio_num 超声波模块的触发引脚
 */
void ultrasonic_init(gpio_num_t trig_gpio_num)
{
    gpio_config_t gpio_config_struct = {0};

    gpio_config_struct.pin_bit_mask = 1ULL << trig_gpio_num;
    gpio_config_struct.intr_type = GPIO_INTR_DISABLE;
    gpio_config_struct.mode = GPIO_MODE_OUTPUT;
    gpio_config_struct.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&gpio_config_struct);

    gpio_set_level(trig_gpio_num, 0);
}

/**
 * @brief 超声波模块的触发函数
 * 
 * @param trig_gpio_num 超声波模块的触发引脚
 */
void ultrasonic_trigger(gpio_num_t trig_gpio_num)
{
    gpio_set_level(trig_gpio_num, 0);
    esp_rom_delay_us(1);
    gpio_set_level(trig_gpio_num, 1);
    esp_rom_delay_us(10);
    gpio_set_level(trig_gpio_num, 0);
}