#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "driver/gpio.h"
#include "driver/uart.h"

#define TX_BUF_SIZE         1024                                                // 串口发送缓冲区大小
#define RX_BUF_SIZE         1024                                                // 串口接收缓冲区大小

void bsp_uart_init(uart_port_t uart_port, uint32_t baud_rate, gpio_num_t tx_gpio_num, int rx_gpio_num);

#endif // !__BSP_UART_H__