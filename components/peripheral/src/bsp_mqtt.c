#include "bsp_mqtt.h"

bool g_mqtt_connect_flag = false;                                               // MQTT连接标志位

esp_mqtt_client_handle_t g_mqtt_client_handle;

static void bsp_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void bsp_mqtt_client_init(void)
{
    // 设置客户端的信息量
    esp_mqtt_client_config_t mqtt_client_config = 
    {
        .broker.address.hostname = MQTT_HOSTNAME,                               // MQTT服务器的域名
        .broker.address.port = MQTT_PORT,                                       // MQTT服务器的端口号
        .broker.address.transport = MQTT_TRANSPORT_OVER_TCP,                    // TCP模式
        .credentials.username = MQTT_CLIENT_USERNAME,                           // 客户端用户名
        .credentials.authentication.password = MQTT_CLIENT_PASSWORD,            // 客户端密码
        .credentials.client_id = MQTT_CLIENT_ID,                                // 客户端ID
    };

    g_mqtt_client_handle = esp_mqtt_client_init(&mqtt_client_config);           // 初始化MQTT客户端

    // 注册MQTT事件处理程序
    esp_mqtt_client_register_event(g_mqtt_client_handle, ESP_EVENT_ANY_ID, bsp_mqtt_event_handler, NULL);

    esp_mqtt_client_start(g_mqtt_client_handle);                                // 连接MQTT服务器 发布数据
}

/**
 * @brief 注册接收MQTT事件的事件处理程序
 * 
 * @param handler_args 注册到事件的用户数据
 * @param base 处理程序的事件库
 * @param event_id 接收到的事件的id
 * @param event_data 事件的数据
 */
static void bsp_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event_id)
    {
        case MQTT_EVENT_CONNECTED:                                              // 连接事件
            g_mqtt_connect_flag = true;
            printf("mqtt connected\n");
            break;

        case MQTT_EVENT_DISCONNECTED:                                           // 断开连接事件
            g_mqtt_connect_flag = false;
            printf("mqtt disconnected\n");
            esp_mqtt_client_reconnect(client);                                  // 重连MQTT服务器
            break;

        case MQTT_EVENT_SUBSCRIBED:                                             // 订阅事件
            printf("mqtt subscribed ack\n");
            break;

        case MQTT_EVENT_PUBLISHED:                                              // 发布事件
            printf("mqtt published ack\n");
            break;

        case MQTT_EVENT_DATA:                                                   // 接收数据事件
            printf("topic: %.*s\n", event->topic_len, event->topic);
            printf("data: %.*s\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_ANY:                                                    // 其它事件
            printf("mqtt event any\n");
            break;
    }
}