#include "bsp_udp.h"

bool g_udp_connect_flag = false;                                                // UDP连接标志位

socklen_t g_udp_socket_fd;                                                      // 套接字描述符
struct sockaddr_in g_udp_remote_address;                                        // 远端地址

uint8_t g_udp_receive_buff[UDP_RECEIVE_BUFF_SIZE];                              // 接收数据缓冲区
uint16_t g_udp_receive_buff_length;                                             // 接收数据缓冲区大小

#define UDP_SEND_DATA_TASK_PRIORITY         5                                   // 发送数据任务优先级
#define UDP_SEND_DATA_TASK_STACK_SIZE       4096                                // 发送数据任务栈大小
TaskHandle_t g_udp_send_data_task_handle;                                       // 发送数据任务句柄
void udp_send_data_task(void *pvParameters);                                    // 发送数据任务函数

#define UDP_RECEIVE_DATA_TASK_PRIORITY      5                                   // 接收数据任务优先级
#define UDP_RECEIVE_DATA_TASK_STACK_SIZE    4096                                // 接收数据任务栈大小
TaskHandle_t g_udp_receive_data_task_handle;                                    // 接收数据任务句柄
void udp_receive_data_task(void *pvParameters);                                 // 接收数据任务函数

/**
 * @brief UDP初始化函数
 * 
 * @param port 本地端口号
 * 
 * @note 该函数会创建一个UDP套接字，并绑定到指定的本地端口上。
 *       同时创建两个任务，一个用于接收数据，一个用于发送数据。
 */
void bsp_udp_init(uint16_t port)
{
    int result = 0;

    struct sockaddr_in udp_address = {
        .sin_addr.s_addr = htons(INADDR_ANY),                                   // 设置本地IP地址
        .sin_port = htons(port),                                                // 设置端口号
        .sin_family = AF_INET,                                                  // IPv4地址
    };
  
    g_udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);                           // 创建UDP套接字
  
    // 绑定本地的地址信息
    result = bind(g_udp_socket_fd, (struct sockaddr *)&udp_address, sizeof(udp_address));
    if (result)
    {
        printf("udp bind socket failed\n");
        closesocket(g_udp_socket_fd);                                           // 关闭套接字
    }
    else
    {
        printf("udp bind socket success\n");

        // 创建发送数据任务
        xTaskCreate(
            udp_send_data_task,                                                 // 任务函数
            "udp_send_data_task",                                               // 任务名
            UDP_SEND_DATA_TASK_STACK_SIZE,                                      // 任务栈大小
            NULL,                                                               // 任务参数
            UDP_SEND_DATA_TASK_PRIORITY,                                        // 任务优先级
            &g_udp_send_data_task_handle                                        // 任务句柄
        );

        // 创建接收数据任务
        xTaskCreate(
            udp_receive_data_task,                                              // 任务函数
            "udp_receive_data_task",                                            // 任务名
            UDP_RECEIVE_DATA_TASK_STACK_SIZE,                                   // 任务栈大小
            NULL,                                                               // 任务参数
            UDP_RECEIVE_DATA_TASK_PRIORITY,                                     // 任务优先级
            &g_udp_receive_data_task_handle                                     // 任务句柄
        );
    }
}

/**
 * @brief UDP发送数据任务函数
 * 
 * @param pvParameters 任务参数
 */
void udp_send_data_task(void *pvParameters)
{
    uint32_t result = 0;

    while (1)
    {
        result = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);                       // 接收任务通知
        if (result && g_udp_receive_buff_length)
        {
            sendto(
                g_udp_socket_fd,                                                // scoket
                g_udp_receive_buff,                                             // 发送数据缓冲区
                strlen((char *)g_udp_receive_buff),                             // 发送的数据大小
                0,                                                              // 数据发送时的特殊处理
                (struct sockaddr *)&g_udp_remote_address,                       // 接收端地址信息
                sizeof(g_udp_remote_address)                                    // 接收端地址信息大小
            );                                                                  // 发送数据

            memset(g_udp_receive_buff, 0, sizeof(g_udp_receive_buff));
            g_udp_receive_buff_length = 0;
        }
    }
}

/**
 * @brief UDP接收数据任务函数
 * 
 * @param pvParameters 任务参数
 */
void udp_receive_data_task(void *pvParameters)
{
    // 如果要获取发送端的地址信息，则该参数的值是一个sockaddr结构体的大小，如果不需要，则该参数的值是0
    socklen_t from_length = sizeof(g_udp_remote_address);

    while (1)
    {
        g_udp_receive_buff_length = recvfrom(
            g_udp_socket_fd,                                                    // socket
            (void *)g_udp_receive_buff,                                         // 接收数据缓冲区
            sizeof(g_udp_receive_buff),                                         // 接收数据缓冲区大小
            0,                                                                  // 用户控制接收的方式
            (struct sockaddr *)&g_udp_remote_address,                           // 保存发送端的地址信息
            &from_length                                                        // 保存接收端地址信息大小 
        );

        if (g_udp_receive_buff_length)
        {
            printf("%s\r\n",g_udp_receive_buff);
            xTaskNotifyGive(g_udp_send_data_task_handle);                       // 发送任务通知
        }
    }
}