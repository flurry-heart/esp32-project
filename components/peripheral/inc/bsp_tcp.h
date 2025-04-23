#ifndef __BSP_TCP_H__
#define __BSP_TCP_H__

#include <sys/socket.h>

#include "freertos/task.h"

#define TCP_RECEIVE_BUFF_SIZE               200                                 // 最大接收数据长度

extern bool g_tcp_connect_flag;                                                 // TCP连接标志位

extern socklen_t g_tcp_socket_fd;                                               // 套接字描述符

extern uint8_t g_tcp_receive_buff[TCP_RECEIVE_BUFF_SIZE];                       // 接收数据缓冲区
extern uint16_t g_tcp_receive_buff_length;                                      // 接收数据长度

void bsp_tcp_client_init(uint16_t port, char *server_ip, uint16_t server_port);
void bsp_tcp_server_init(uint16_t port, int max_connect_count);

#endif // !__BSP_TCP_H__