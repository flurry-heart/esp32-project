#ifndef __BSP_UDP_H__
#define __BSP_UDP_H__

#include <sys/socket.h>

#include "freertos/task.h"

#define UDP_RECEIVE_BUFF_SIZE               200                                 // 最大接收数据长度

extern bool g_udp_connect_flag;                                                 // UDP连接标志位

extern socklen_t g_udp_socket_fd;                                               // 套接字描述符
extern struct sockaddr_in g_udp_remote_address;                                 // 远端地址

extern uint8_t g_udp_receive_buff[UDP_RECEIVE_BUFF_SIZE];                       // 接收数据缓冲区
extern uint16_t g_udp_receive_buff_length;                                      // 接收数据缓冲区大小

void bsp_udp_init(uint16_t port);

#endif // !__BSP_UDP_H__