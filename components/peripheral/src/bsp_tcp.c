#include "bsp_tcp.h"

bool g_tcp_connect_flag = false;                                                // TCP连接标志位

socklen_t g_tcp_socket_fd;                                                      // 套接字描述符

QueueHandle_t g_semphore_handle;

uint8_t g_tcp_receive_buff[TCP_RECEIVE_BUFF_SIZE];                              // 接收数据缓冲区
uint16_t g_tcp_receive_buff_length;                                             // 接收数据长度

#define TCP_SEND_DATA_TASK_PRIORITY         1                                   // 发送数据任务优先级
#define TCP_SEND_DATA_TASK_STACK_SIZE       4096                                // 发送数据任务栈大小
TaskHandle_t g_tcp_send_data_task_handle;                                       // 发送数据任务句柄
void tcp_send_data_task(void *pvParameters);                                    // 发送数据任务函数

#define TCP_RECEIVE_DATA_TASK_PRIORITY      1                                   // 接收数据任务优先级
#define TCP_RECEIVE_DATA_TASK_STACK_SIZE    4096                                // 接收数据任务栈大小
TaskHandle_t g_tcp_receive_data_task_handle;                                    // 接收数据任务句柄
void tcp_receive_data_task(void *pvParameters);                                 // 接收数据任务函数

/**
 * @brief TCP客户端初始化函数
 * 
 * @param port 客户端端口号
 * @param server_ip 服务端IP地址
 * @param server_port 服务端端口号
 * 
 * @note 该函数会创建一个TCP套接字，并绑定到指定的客户端端口上。
 *       同时创建两个任务，一个用于接收数据，一个用于发送数据。
 */
void bsp_tcp_client_init(uint16_t port, char *server_ip, uint16_t server_port)
{
    int result = 0;

    g_semphore_handle = xSemaphoreCreateCounting(30, 0);                        // 创建计数型信号量

    struct sockaddr_in tcp_client_address = 
    {
        .sin_addr.s_addr = htonl(INADDR_ANY),                                   // 客户端IP地址
        .sin_port = htons(port),                                                // 客户端端口号
        .sin_family = AF_INET                                                   // IPv4地址
    };

    struct sockaddr_in tcp_server_address = 
    {
        .sin_addr.s_addr = inet_addr(server_ip),                                // 服务端IP地址
        .sin_port = htons(server_port),                                         // 服务端端口号
        .sin_family = AF_INET                                                   // IPv4地址
    };
  
    g_tcp_socket_fd = socket(AF_INET, SOCK_STREAM, 0);                          // 创建TCP套接字

    // 绑定本地的地址信息到套接字中
    bind(g_tcp_socket_fd, (struct sockaddr *)&tcp_client_address, sizeof(tcp_client_address));

    // 连接服务器
    result = connect(g_tcp_socket_fd, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address));
    if (result)
    {
        printf("connect server failed\n");
        closesocket(g_tcp_socket_fd);                                           // 关闭套接字
    }
    else
    {
        printf("connect server success\n");

        // 创建发送数据任务
        xTaskCreate(
            tcp_send_data_task,                                                 // 任务函数
            "tcp_send_data_task",                                               // 任务名
            TCP_SEND_DATA_TASK_STACK_SIZE,                                      // 任务栈大小
            (void *)g_tcp_socket_fd,                                            // 任务参数
            TCP_SEND_DATA_TASK_PRIORITY,                                        // 任务优先级
            &g_tcp_send_data_task_handle                                        // 任务句柄
        );

        // 创建接收数据任务
        xTaskCreate(
            tcp_receive_data_task,                                              // 任务函数
            "tcp_receive_data_task",                                            // 任务名
            TCP_RECEIVE_DATA_TASK_STACK_SIZE,                                   // 任务栈大小
            (void *)g_tcp_socket_fd,                                            // 任务参数
            TCP_RECEIVE_DATA_TASK_PRIORITY,                                     // 任务优先级
            &g_tcp_receive_data_task_handle                                     // 任务句柄
        );
    }
}

/**
 * @brief TCP服务端初始化函数
 * 
 * @param port 服务端端口号
 * @param max_connect_count 最大连接数
 * 
 * @note 每一个客户端连接服务端时会创建两个任务，一个用于接收数据，一个用于发送数据。
 */
void bsp_tcp_server_init(uint16_t port, int max_connect_count)
{
    int result = 0;

    g_semphore_handle = xSemaphoreCreateCounting(30, 0);                        // 创建计数型信号量

    struct sockaddr_in tcp_server_address = 
    {
        .sin_addr.s_addr = htonl(INADDR_ANY),                                   // 客户端IP地址
        .sin_port = htons(port),                                                // 客户端端口号
        .sin_family = AF_INET                                                   // IPv4地址
    };
  
    g_tcp_socket_fd = socket(AF_INET, SOCK_STREAM, 0);                          // 创建TCP套接字

    // 绑定本地的地址信息到套接字中
    bind(g_tcp_socket_fd, (struct sockaddr *)&tcp_server_address, sizeof(tcp_server_address));

    // 开始监听
    result = listen(g_tcp_socket_fd, max_connect_count);                        // 监听套接字
    if (result)
    {
        printf("server listen failed\n");
        closesocket(g_tcp_socket_fd);                                           // 关闭套接字
    }
    else
    {
        printf("server listen success\n");

        while (1)
        {
            struct sockaddr_in tcp_client_address = {0};
            socklen_t client_address_length = sizeof(tcp_client_address);       // 客户端地址长度

            // 等待客户端连接
            socklen_t new_tcp_fd = accept(g_tcp_socket_fd, (struct sockaddr *)&tcp_client_address, &client_address_length);
            if (new_tcp_fd != -1)
            {
                // 获取客户端 IP 地址
                char client_str[128] = {0};
                char client_send_task_name[128] = {0};
                char client_receive_task_name[128] = {0};

                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &tcp_client_address.sin_addr, client_ip, sizeof(client_ip));

                sprintf(client_str, "client(%s:%d)", client_ip, tcp_client_address.sin_port);
                sprintf(client_send_task_name, "%s send data task", client_str);
                sprintf(client_receive_task_name, "%s receive data task", client_str);
                printf("%s connect success\n", client_str);

                // 每连接一个客户端就创建一个发送给客户端的任务
                xTaskCreate(
                    tcp_send_data_task,                                         // 任务函数
                    client_send_task_name,                                      // 任务名
                    TCP_SEND_DATA_TASK_STACK_SIZE,                              // 任务栈大小
                    (void *)new_tcp_fd,                                         // 任务参数
                    TCP_SEND_DATA_TASK_PRIORITY,                                // 任务优先级
                    NULL                                                        // 任务句柄
                );

                xTaskCreate(
                    tcp_receive_data_task,                                      // 任务函数
                    client_receive_task_name,                                   // 任务名
                    TCP_RECEIVE_DATA_TASK_STACK_SIZE,                           // 任务栈大小
                    (void *)new_tcp_fd,                                         // 任务参数
                    TCP_RECEIVE_DATA_TASK_PRIORITY,                             // 任务优先级
                    NULL                                                        // 任务句柄
                );
            }
        }
    }
}

/**
 * @brief TCP发送数据任务函数
 * 
 * @param pvParameters 任务参数
 */
void tcp_send_data_task(void *pvParameters)
{
    uint32_t result = 0;
    socklen_t tcp_socket_fd = (socklen_t)pvParameters;                          // 获取任务参数

    while (1)
    {
        result = xSemaphoreTake(g_semphore_handle, 1000);                       // 获取信号量
        if (result && g_tcp_receive_buff_length)
        {
            send(
                tcp_socket_fd,                                                  // scoket
                g_tcp_receive_buff,                                             // 发送数据缓冲区
                strlen((char *)g_tcp_receive_buff),                             // 发送的数据大小
                0                                                               // 数据发送时的特殊处理
            );

            if (strcmp((char *)g_tcp_receive_buff, "exit") == 0)                // 如果接收到的字符串是"exit"
            {
                vTaskDelete(NULL);                                              // 删除任务
            }

            memset(g_tcp_receive_buff, 0, sizeof(g_tcp_receive_buff));
        }
    }
}

/**
 * @brief TCP接收数据任务函数
 * 
 * @param pvParameters 任务参数
 */
void tcp_receive_data_task(void *pvParameters)
{
    socklen_t tcp_socket_fd = (socklen_t)pvParameters;                          // 获取任务参数

    while (1)
    {
        g_tcp_receive_buff_length = recv(
            tcp_socket_fd,                                                      // socket
            (void *)g_tcp_receive_buff,                                         // 接收数据缓冲区
            sizeof(g_tcp_receive_buff),                                         // 接收数据缓冲区大小
            0                                                                   // 用户控制接收的方式
        );

        if (g_tcp_receive_buff_length)
        {
            printf("%s\r\n",g_tcp_receive_buff);
            xSemaphoreGive(g_semphore_handle);                                  // 释放计数型信号量
        }

        if (strcmp((char *)g_tcp_receive_buff, "exit") == 0)                    // 如果接收到的字符串是"exit"
        {
            printf("client exit\n");
            closesocket(tcp_socket_fd);                                         // 关闭套接字
            vTaskDelete(NULL);                                                  // 删除任务
        }
    }
}