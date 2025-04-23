#include "bsp_ledc.h"

/**
 * @brief LEDC初始化
 * 
 * @param time_num 定时器编号
 * @param ledc_channel LEDC通道编号
 * @param gpio_num GPIO引脚编号
 * @param resolution LEDC占空比分辨率
 * @param freq LEDC频率
 * @param duty LEDC占空比
 */
void bsp_ledc_init(ledc_timer_t time_num, ledc_channel_t ledc_channel, int gpio_num, ledc_timer_bit_t resolution, uint32_t freq, uint32_t duty)
{
    ledc_timer_config_t ledc_timer_config_struct = {0};
    ledc_channel_config_t ledc_channel_config_struct = {0};

    // 配置LEDC定时器
    ledc_timer_config_struct.clk_cfg = LEDC_AUTO_CLK;                           // LEDC时钟源选择
    ledc_timer_config_struct.timer_num = time_num;                              // PWM定时器编号
    ledc_timer_config_struct.duty_resolution = resolution;                      // PWM占空比分辨率
    ledc_timer_config_struct.freq_hz = freq;                                    // PWM频率
    ledc_timer_config_struct.speed_mode = LEDC_LOW_SPEED_MODE;                  // LEDC控制器工作模式
    ledc_timer_config(&ledc_timer_config_struct);                               // 配置定时器

    // 配置LEDC通道
    ledc_channel_config_struct.timer_sel = time_num;                            // LEDC控制器通道对应定时器编号
    ledc_channel_config_struct.gpio_num = gpio_num;                             // LEDC控制器通道对应引脚
    ledc_channel_config_struct.channel = ledc_channel;                          // LEDC控制器通道编号
    ledc_channel_config_struct.speed_mode = LEDC_LOW_SPEED_MODE;                // LEDC控制器通道工作模式
    ledc_channel_config_struct.intr_type = LEDC_INTR_DISABLE;                   // LEDC失能中断
    ledc_channel_config_struct.duty = duty;                                     // LEDC通道占空比
    ledc_channel_config(&ledc_channel_config_struct);                           // 配置通道
}

/**
 * @brief LEDC设置占空比
 * 
 * @param ledc_channel LEDC通道编号
 * @param duty LEDC占空比
 */
void bsp_ledc_set_pwm_duty(ledc_channel_t ledc_channel, uint32_t duty)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, ledc_channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, ledc_channel);
}

/**
 * @brief LEDC设置渐变占空比
 * 
 * @param ledc_channel LEDC通道编号
 * @param start_value 起始占空比
 * @param end_value 结束占空比
 * @param gradient_time 渐变时长
 */
void bsp_ledc_set_gradient_range(ledc_channel_t ledc_channel, uint32_t start_value, uint32_t end_value, int gradient_time)
{
    // 设置占空比以及渐变时长
    ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, ledc_channel, start_value, gradient_time);
    // 开始渐变
    ledc_fade_start(LEDC_LOW_SPEED_MODE, ledc_channel, LEDC_FADE_NO_WAIT);
    
    ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, ledc_channel, end_value, gradient_time);
    ledc_fade_start(LEDC_LOW_SPEED_MODE, ledc_channel, LEDC_FADE_NO_WAIT);
}