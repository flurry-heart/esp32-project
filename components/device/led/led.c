#include "led.h"

/**
 * @brief LED初始化
 * 
 * @param gpio_num GPIO引脚
 */
void led_init(gpio_num_t gpio_num)
{
    gpio_config_t gpio_config_struct = {0};

    gpio_config_struct.pin_bit_mask = 1ULL << gpio_num;                         // 设置引脚
    gpio_config_struct.intr_type = GPIO_INTR_DISABLE;                           // 不使用中断
    gpio_config_struct.mode = GPIO_MODE_INPUT_OUTPUT;                           // 输入输出模式
    gpio_config_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;                    // 不使用下拉
    gpio_config_struct.pull_up_en = GPIO_PULLUP_DISABLE;                        // 不使用上拉
    gpio_config(&gpio_config_struct);                                           // 配置GPIO
}

/**
 * @brief LED设置电平
 * 
 * @param gpio_num GPIO引脚
 * @param status 引脚电平
 */
void led_set_status(gpio_num_t gpio_num, uint8_t status)
{
    gpio_set_level(gpio_num, status);                                           // 设置引脚电平
}

/**
 * @brief LED翻转电平
 * 
 * @param gpio_num GPIO引脚
 */
void led_toggle_status(gpio_num_t gpio_num)
{
    // 如果引脚未配置为输入模式或输入输出模式，gpio_get_level()函数始终返回0
    gpio_set_level(gpio_num, !gpio_get_level(gpio_num));
}
