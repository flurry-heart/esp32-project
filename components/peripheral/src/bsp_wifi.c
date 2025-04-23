#include "bsp_wifi.h"

bool g_wifi_connect_flag = false;                                               // WiFi连接标志位

static void bsp_wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/**
 * @brief 打印身份认证模式函数
 * 
 * @param authmode 身份验证模式
 */
void print_auth_mode(int authmode)
{
    switch (authmode)
    {
    case WIFI_AUTH_OPEN:
        printf("Authmode: WIFI_AUTH_OPEN\n");
        break;

    case WIFI_AUTH_OWE:
        printf("Authmode: WIFI_AUTH_OWE\n");
        break;

    case WIFI_AUTH_WEP:
        printf("Authmode: WIFI_AUTH_WEP\n");
        break;
    case WIFI_AUTH_WPA_PSK:
        printf("Authmode: WIFI_AUTH_WPA_PSK\n");
        break;

    case WIFI_AUTH_WPA2_PSK:
        printf("Authmode: WIFI_AUTH_WPA2_PSK\n");
        break;

    case WIFI_AUTH_WPA_WPA2_PSK:
        printf("Authmode: WIFI_AUTH_WPA_WPA2_PSK\n");
        break;

    case WIFI_AUTH_ENTERPRISE:
        printf("Authmode: WIFI_AUTH_ENTERPRISE\n");
        break;

    case WIFI_AUTH_WPA3_PSK:
        printf("Authmode: WIFI_AUTH_WPA3_PSK\n");
        break;

    case WIFI_AUTH_WPA2_WPA3_PSK:
        printf("Authmode: WIFI_AUTH_WPA2_WPA3_PSK\n");
        break;

    case WIFI_AUTH_WPA3_ENTERPRISE:
        printf("Authmode: WIFI_AUTH_WPA3_ENTERPRISE\n");
        break;

    case WIFI_AUTH_WPA2_WPA3_ENTERPRISE:
        printf("Authmode: WIFI_AUTH_WPA2_WPA3_ENTERPRISE\n");
        break;

    case WIFI_AUTH_WPA3_ENT_192:
        printf("Authmode: WIFI_AUTH_WPA3_ENT_192\n");
        break;

    default:
        printf("Authmode: WIFI_AUTH_UNKNOWN\n");
        break;
    }
}

/**
 * @brief 打印WIFI密码类型函数
 * 
 * @param pairwise_cipher 密码类型
 * @param group_cipher 群密码类型
 */
void print_cipher_type(int pairwise_cipher, int group_cipher)
{
    switch (pairwise_cipher)
    {
    case WIFI_CIPHER_TYPE_NONE:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_NONE\n");
        break;

    case WIFI_CIPHER_TYPE_WEP40:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_WEP40\n");
        break;

    case WIFI_CIPHER_TYPE_WEP104:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_WEP104\n");
        break;

    case WIFI_CIPHER_TYPE_TKIP:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_TKIP\n");
        break;

    case WIFI_CIPHER_TYPE_CCMP:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP\n");
        break;

    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_TKIP_CCMP\n");
        break;

    case WIFI_CIPHER_TYPE_AES_CMAC128:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_AES_CMAC128\n");
        break;

    case WIFI_CIPHER_TYPE_SMS4:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_SMS4\n");
        break;

    case WIFI_CIPHER_TYPE_GCMP:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_GCMP\n");
        break;

    case WIFI_CIPHER_TYPE_GCMP256:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_GCMP256\n");
        break;

    default:
        printf("Pairwise Cipher: WIFI_CIPHER_TYPE_UNKNOWN\n");
        break;
    }

    switch (group_cipher)
    {
    case WIFI_CIPHER_TYPE_NONE:
        printf("Group Cipher: WIFI_CIPHER_TYPE_NONE\n");
        break;

    case WIFI_CIPHER_TYPE_WEP40:
        printf("Group Cipher: WIFI_CIPHER_TYPE_WEP40\n");
        break;

    case WIFI_CIPHER_TYPE_WEP104:
        printf("Group Cipher: WIFI_CIPHER_TYPE_WEP104\n");
        break;

    case WIFI_CIPHER_TYPE_TKIP:
        printf("Group Cipher: WIFI_CIPHER_TYPE_TKIP\n");
        break;

    case WIFI_CIPHER_TYPE_CCMP:
        printf("Group Cipher: WIFI_CIPHER_TYPE_CCMP\n");
        break;

    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        printf("Group Cipher: WIFI_CIPHER_TYPE_TKIP_CCMP\n");
        break;

    case WIFI_CIPHER_TYPE_SMS4:
        printf("Group Cipher: WIFI_CIPHER_TYPE_SMS4\n");
        break;

    case WIFI_CIPHER_TYPE_GCMP:
        printf("Group Cipher: WIFI_CIPHER_TYPE_GCMP\n");
        break;

    case WIFI_CIPHER_TYPE_GCMP256:
        printf("Group Cipher: WIFI_CIPHER_TYPE_GCMP256\n");
        break;

    default:
        printf("Group Cipher: WIFI_CIPHER_TYPE_UNKNOWN\n");
        break;
    }
}

/**
 * @brief 扫描WiFi函数
 * 
 */
void bsp_wifi_scan(void)
{
    esp_netif_init();                                                           // 网卡初始化
    esp_event_loop_create_default();                                            // 创建新的事件循环

    esp_netif_create_default_wifi_sta();                                        // 使用默认的设置初始化STA模式

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);                                           // WiFi配置初始化

    uint16_t number = DEFAULT_SCAN_WIFI_LIST_COUNT;                             // 默认扫描列表大小
    wifi_ap_record_t ap_info[DEFAULT_SCAN_WIFI_LIST_COUNT];
    memset(ap_info, 0, sizeof(ap_info));

    esp_wifi_set_mode(WIFI_MODE_STA);                                           // 设置WiFi模式为STA模式
    esp_wifi_start();                                                           // 启动WiFi

    esp_wifi_scan_start(NULL, true);                                            // 开始扫描附近的WiFi

    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);                                        // 获取上次扫描中找到的AP数量
    esp_wifi_scan_get_ap_records(&number, ap_info);                             // 获取上次扫描中找到的AP列表

    printf("Total APs scanned = %u\n\n", ap_count);

    // 下面是打印附近的WiFi信息
    for (int i = 0; (i < DEFAULT_SCAN_WIFI_LIST_COUNT) && (i < ap_count); i++)
    {
        printf("SSID: %s\n", ap_info[i].ssid);
        printf("RSSI: %d\n", ap_info[i].rssi);

        print_auth_mode(ap_info[i].authmode);
        
        if (ap_info[i].authmode != WIFI_AUTH_WEP)
        {
            print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
        }

        printf("Channel: %d\n\n", ap_info[i].primary);
    }
}

/**
 * @brief 连接WiFi函数
 * 
 * @param ssid WiFi名称
 * @param password WiFi密码
 * @param auth_mode WiFi认证模式
 */
void bsp_wifi_sta_init(char *ssid, char *password, wifi_auth_mode_t auth_mode)
{
    wifi_config_t wifi_config = {
        .sta.threshold.authmode = auth_mode,                                    // 认证模式
    };

    memset(wifi_config.sta.ssid, 0, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.ssid, ssid, strlen(ssid));                           // ssid

    memset(wifi_config.sta.password, 0, sizeof(wifi_config.sta.password));
    memcpy(wifi_config.sta.password, password, strlen(password));               // 密码

    esp_netif_init();                                                           // 网卡初始化
    esp_event_loop_create_default();                                            // 创建新的事件循环

    esp_netif_create_default_wifi_sta();                                        // 使用默认的设置初始化STA模式

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);                                           // WiFi配置初始化

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &bsp_wifi_event_handler, NULL);    // 注册WiFi事件处理函数
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &bsp_wifi_event_handler, NULL);   // 注册IP事件处理函数

    esp_wifi_set_mode(WIFI_MODE_STA);                                           // 设置WiFi模式为STA模式
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);                         // 设置WiFi配置

    esp_wifi_start();                                                           // 启动WiFi
}

/**
 * @brief 创建WiFi热点函数
 * 
 * @param ssid WiFi热点名称
 * @param password WiFi热点密码
 * @param auth_mode WiFi热点认证模式
 * @param max_connection WiFi热点最大连接数
 */
void bsp_wifi_ap_init(char *ssid, char *password, wifi_auth_mode_t auth_mode, uint8_t max_connection)
{
    esp_netif_init();                                                           // 初始化网卡
    esp_event_loop_create_default();                                            // 创建新的事件循环

    esp_netif_create_default_wifi_ap();                                         // 使用默认配置初始化AP模式

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);                                           // WiFi配置初始化

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &bsp_wifi_event_handler, NULL);    // 注册WiFi事件处理函数

    // 配置热点信息
    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len = strlen(ssid),                                           // ssid长度
            .authmode = auth_mode,                                              // 认证模式
            .max_connection = max_connection,                                   // 最大连接数
        },
    };

    if (strlen(password) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    memset(wifi_config.ap.ssid, 0, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.ap.ssid, ssid, strlen(ssid));                            // ssid

    memset(wifi_config.ap.password, 0, sizeof(wifi_config.sta.password));
    memcpy(wifi_config.ap.password, password, strlen(password));                // 密码

    esp_wifi_set_mode(WIFI_MODE_AP);                                            // 设置WiFi模式为AP模式
    esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);                          // 设置WiFi配置
    esp_wifi_start();                                                           // 启动WiFi

    printf("wifi ap init finished. SSID: %s, password: %s\n", ssid, password);
}

/**
 * @brief SmartConfig一键配网函数
 * 
 * @param type 一键配网方式
 */
void bsp_wifi_smartconfig_init(smartconfig_type_t type)
{
    esp_netif_init();                                                           // 初始化网卡
    esp_event_loop_create_default();                                            // 创建新的事件循环

    esp_netif_create_default_wifi_sta();                                        // 用默认的设置初始化STA模式

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);                                           // WiFi初始化

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &bsp_wifi_event_handler, NULL);    // 注册WiFi事件处理函数
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &bsp_wifi_event_handler, NULL);   // 注册IP事件处理函数
    esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &bsp_wifi_event_handler, NULL);      // 注册SmartConfig事件处理函数

    esp_wifi_set_mode(WIFI_MODE_STA);                                            // 设置WiFi模式为STA模式

    esp_wifi_start();                                                           // 启动WiFi

    esp_smartconfig_set_type(type);                                             // 设置smartconfig的类型

    smartconfig_start_config_t smart_config = SMARTCONFIG_START_CONFIG_DEFAULT();
    esp_smartconfig_start(&smart_config);
}

/**
 * @brief WiFi事件处理函数
 * 
 * @param arg 用户自定义参数，通常为NULL
 * @param event_base 事件的基础类型，用于区分不同的事件源（如WiFi事件、IP事件等）
 * @param event_id 事件的ID，用于区分同一事件源下的不同事件
 * @param event_data 事件相关的数据，具体内容取决于事件类型
 */
static void bsp_wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT)                                               // WiFi事件
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:                                              // WiFi启动STA模式事件
            g_wifi_connect_flag = false;
            esp_wifi_connect();                                                 // 启动WiFi连接
            break;

        case WIFI_EVENT_STA_CONNECTED:                                          // WiFi连接成功事件
            // g_wifi_connect_flag = true;
            printf("connect to ap success\n");
            break;

        case WIFI_EVENT_STA_DISCONNECTED:                                       // WiFi断开连接事件
            g_wifi_connect_flag = false;
            esp_wifi_connect();
            printf("retry to connect to the AP\n");
            break;

        case WIFI_EVENT_AP_STACONNECTED:                                        // 设备连接事件
            wifi_event_ap_staconnected_t *wifi_ap_connect_event = (wifi_event_ap_staconnected_t *)event_data;
            printf("station(%02x:%02x:%02x:%02x:%02x:%02x) join, AID=%d", 
                wifi_ap_connect_event->mac[0], wifi_ap_connect_event->mac[1], wifi_ap_connect_event->mac[2], 
                wifi_ap_connect_event->mac[3], wifi_ap_connect_event->mac[4], wifi_ap_connect_event->mac[5], 
                wifi_ap_connect_event->aid);
            break;

        case WIFI_EVENT_AP_STADISCONNECTED:                                     // 设备断开事件
            wifi_event_ap_stadisconnected_t *wifi_ap_disconnect_event = (wifi_event_ap_stadisconnected_t *)event_data;
            printf("station(%02x:%02x:%02x:%02x:%02x:%02x) leave, AID=%d", 
                wifi_ap_disconnect_event->mac[0], wifi_ap_disconnect_event->mac[1], wifi_ap_disconnect_event->mac[2], 
                wifi_ap_disconnect_event->mac[3], wifi_ap_disconnect_event->mac[4], wifi_ap_disconnect_event->mac[5], 
                wifi_ap_disconnect_event->aid);
            break;
        }
    }
    else if (event_base == IP_EVENT)                                            // 获取IP事件
    {
        switch (event_id)
        {
        case IP_EVENT_STA_GOT_IP:                                               // 获取从路由器分配的IP地址事件
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            printf("get ip:" IPSTR "\n", IP2STR(&event->ip_info.ip));
            g_wifi_connect_flag = true;
            break;
        }
    }
    else if (event_base == SC_EVENT)                                            // 获取SmartConfig事件
    {
        switch (event_id)
        {
        case SC_EVENT_GOT_SSID_PSWD:                                             // 获取SSID和密码事件
            smartconfig_event_got_ssid_pswd_t *smart_config_event = (smartconfig_event_got_ssid_pswd_t *)event_data;

            wifi_config_t wifi_config = {
                .sta.bssid_set = smart_config_event->bssid_set,                 // 是否需要设置MAC地址
            };

            memset(&wifi_config.sta.ssid, 0, sizeof(wifi_config.sta.ssid));
            memcpy(wifi_config.sta.ssid, smart_config_event->ssid, sizeof(wifi_config.sta.ssid));

            memset(&wifi_config.sta.password, 0, sizeof(wifi_config.sta.password));
            memcpy(wifi_config.sta.password, smart_config_event->password, sizeof(wifi_config.sta.password));

            // 如果需要设置MAC地址，则拷贝
            if (smart_config_event->bssid_set)
            {
                memset(&wifi_config.sta.bssid, 0, sizeof(wifi_config.sta.bssid));
                memcpy(wifi_config.sta.bssid, smart_config_event->bssid, sizeof(wifi_config.sta.bssid));
            }

            esp_wifi_disconnect();                                              // 断开WiFi连接
            esp_wifi_set_config(WIFI_IF_STA, &wifi_config);                     // 设置WiFi配置
            esp_wifi_connect();                                                 // 重新连接WiFi
            break;

        case SC_EVENT_SEND_ACK_DONE:                                            // 发送ACK完成事件
            esp_smartconfig_stop();
            printf("smartconfig done\n");
            break;
        }
    }
}