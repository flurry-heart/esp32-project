#include "bsp_uart.h"

/**
 * @brief 串口初始化
 * 
 * @param uart_port 串口号
 * @param baud_rate 波特率
 * @param tx_gpio_num 串口发送引脚
 * @param rx_gpio_num 串口接收引脚
 */
void bsp_uart_init(uart_port_t uart_port, uint32_t baud_rate, gpio_num_t tx_gpio_num, int rx_gpio_num)
{
    uart_config_t uart_config = {0};

    uart_config.baud_rate = baud_rate;                                          // 波特率
    uart_config.data_bits = UART_DATA_8_BITS;                                   // 数据位
    uart_config.parity = UART_PARITY_DISABLE;                                   // 校验位
    uart_config.stop_bits = UART_STOP_BITS_1;                                   // 停止位
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;                           // 硬件流控
    uart_config.source_clk = UART_SCLK_APB;                                     // 配置时钟源
    uart_config.rx_flow_ctrl_thresh = 122;                                      // 硬件控制流阈值
    uart_param_config(uart_port, &uart_config);

    // 配置uart引脚
    uart_set_pin(uart_port, tx_gpio_num, rx_gpio_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // 安装串口驱动
    uart_driver_install(uart_port, RX_BUF_SIZE, TX_BUF_SIZE, 32, NULL, 0);
}