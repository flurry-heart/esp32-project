#include "key.h"

/**
 * @brief 按键初始化函数
 * 
 * @param gpio_num GPIO 引脚编号
 * @param press_level 按键按下时的电压电平
 */
void key_init(gpio_num_t gpio_num, uint8_t press_level)
{
    gpio_config_t gpio_config_struct = {0};

    gpio_config_struct.pin_bit_mask = 1ULL << gpio_num;                         // 设置引脚
    gpio_config_struct.intr_type = GPIO_INTR_DISABLE;                           // 不使用中断
    gpio_config_struct.mode = GPIO_MODE_INPUT;                                  // 输入模式
    if (press_level) 
    {
        gpio_config_struct.pull_down_en = GPIO_PULLDOWN_ENABLE;                 // 使用下拉
        gpio_config_struct.pull_up_en = GPIO_PULLUP_DISABLE;                    // 不使用上拉
    } 
    else 
    {
        gpio_config_struct.pull_up_en = GPIO_PULLUP_ENABLE;                     // 使用上拉
        gpio_config_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;                // 不使用下拉
    }
    gpio_config(&gpio_config_struct);                                           // 配置GPIO
}

/**
 * @brief 按键扫描函数
 * 
 * @param gpio_num GPIO 引脚编号
 * @param press_level 按键按下时的电压电平
 * @return true 按键按下
 * @return false 按键未按下
 */
bool key_scan(gpio_num_t gpio_num, uint8_t press_level)
{
    if (gpio_get_level(gpio_num) == press_level)
    {
        // 将一个任务延迟给定的滴答数，IDF中提供pdMS_TO_TICKS可以将指定的ms转换为对应的tick数
        vTaskDelay(pdMS_TO_TICKS(10));                                          // 延迟消抖
        if (gpio_get_level(gpio_num) == press_level)                            // 再次检测
        {
            return true;
        }
    }
    
    return false;
}