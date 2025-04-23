#include "rotary_encoder.h"

/**
 * @brief 旋转编码器初始化函数
 * 
 * @param clk_pin A相引脚编号
 * @param dt_pin B相引脚编号
 * @param sw_pin 按键引脚编号
 */
void rotary_encoder_init(gpio_num_t clk_pin, gpio_num_t dt_pin, gpio_num_t sw_pin)
{
    gpio_config_t gpio_config_struct = 
    {
        .pin_bit_mask = (1ULL << clk_pin) | (1ULL << dt_pin) | (1ULL << sw_pin),    // 配置引脚
        .mode = GPIO_MODE_INPUT,                                                // 输入模式
        .pull_up_en = GPIO_PULLUP_ENABLE,                                       // 使能上拉
        .pull_down_en = GPIO_PULLDOWN_DISABLE,                                  // 不使能下拉
        .intr_type = GPIO_INTR_DISABLE                                          // 不使能引脚中断
    };
    gpio_config(&gpio_config_struct);
}

/**
 * @brief 旋转编码器扫描函数
 * 
 * @param clk_pin A相引脚编号
 * @param dt_pin B相引脚编号
 * @param sw_pin 按键引脚编号
 */
uint8_t rotary_encoder_scan(gpio_num_t clk_pin, gpio_num_t dt_pin, gpio_num_t sw_pin)
{
    static uint8_t last_clk_level = 0;                                          // 上一次的CLK状态（A相）
    static uint8_t last_dt_level = 0;                                           // 上一次的DT状态（B相）

    uint8_t result = 0;

    // 当A发生跳变时采集B当前的状态，并将B与上一次的状态进行对比。
    if (gpio_get_level(clk_pin) != last_clk_level)
    {           
        // 若A 0->1 时，B 1->0 正转；B 0->1 反转
        if (gpio_get_level(clk_pin) == 1)
        {
            // B和上一次状态相比，为下降沿，正转
            if ((last_dt_level == 1) && (gpio_get_level(dt_pin) == 0))
            {
                result = 2;
            }
            // B和上一次状态相比，为上升沿，反转
            else if ((last_dt_level == 0) && (gpio_get_level(dt_pin) == 1))
            {
                result = 1;
            }
        }
        // 若A 1->0 时，B 0->1 正转；B 1->0 反转
        else
        {
            // B和上一次状态相比，为下降沿，反转
            if ((last_dt_level == 1) && (gpio_get_level(dt_pin) == 0))
            {
                result = 1;
            }
            // B和上一次状态相比，为上升沿，正转
            else if ((last_dt_level == 0) && (gpio_get_level(dt_pin) == 1))
            {
                result = 2;
            }
        }

        last_clk_level = gpio_get_level(clk_pin);                               // 更新编码器上一个状态暂存变量
        last_dt_level = gpio_get_level(dt_pin);                                 // 更新编码器上一个状态暂存变量
    }

    if (gpio_get_level(sw_pin) == 0)
    {
        result = 3;
    }

    return result;
}