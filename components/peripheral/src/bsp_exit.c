#include "bsp_exit.h"

static void bsp_exit_gpio_isr_handler(void *arg);

/**
 * @brief 外部中断初始化
 * 
 * @param gpio_num GPIO引脚
 * @param trigger_edge 触发边沿
 */
void bsp_exit_init(gpio_num_t gpio_num, uint8_t trigger_edge)
{
    gpio_config_t gpio_config_struct = {0};

    gpio_config_struct.pin_bit_mask = 1ULL << gpio_num;                         // 设置引脚
    gpio_config_struct.mode = GPIO_MODE_INPUT;                                  // 输入模式
    if (trigger_edge) 
    {
        gpio_config_struct.pull_down_en = GPIO_PULLDOWN_ENABLE;                 // 使用下拉
        gpio_config_struct.pull_up_en = GPIO_PULLUP_DISABLE;                    // 不使用上拉
        gpio_config_struct.intr_type = GPIO_INTR_POSEDGE;                       // 上升沿触发
    } 
    else 
    {
        gpio_config_struct.pull_up_en = GPIO_PULLUP_ENABLE;                     // 使用上拉
        gpio_config_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;                // 不使用下拉
        gpio_config_struct.intr_type = GPIO_INTR_NEGEDGE;                       // 下降沿触发
    }
    gpio_config(&gpio_config_struct);                                           // 配置GPIO

    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);                               // 注册中断服务函数
    gpio_isr_handler_add(gpio_num, bsp_exit_gpio_isr_handler, (void *)gpio_num);// 设置GPIO的中断回调函数
    gpio_intr_enable(gpio_num);                                                 // 使能GPIO模块中断信号
}

/**
 * @brief 外部中断服务函数
 * 
 * @param arg 传入的参数
 */
static void IRAM_ATTR bsp_exit_gpio_isr_handler(void *arg)
{
    gpio_num_t gpio_num = (gpio_num_t) arg;

    if (gpio_num == GPIO_NUM_2)
    {
        gpio_set_level(GPIO_NUM_1, !gpio_get_level(GPIO_NUM_1));                // 设置GPIO电平
    }
}