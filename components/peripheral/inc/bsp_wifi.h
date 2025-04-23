#ifndef __BSP_WIFI_H__
#define __BSP_WIFI_H__

#include <stdio.h>
#include <string.h>

#include "esp_wifi.h"
#include "esp_smartconfig.h"

#define DEFAULT_SCAN_WIFI_LIST_COUNT    12                                      // 默认扫描WiFi列表的个数

extern bool g_wifi_connect_flag;                                                // WiFi连接标志位

void print_auth_mode(int authmode);
void print_cipher_type(int pairwise_cipher, int group_cipher);

void bsp_wifi_scan(void);
void bsp_wifi_sta_init(char *ssid, char *password, wifi_auth_mode_t auth_mode);
void bsp_wifi_ap_init(char *ssid, char *password, wifi_auth_mode_t auth_mode, uint8_t max_connection);
void bsp_wifi_smartconfig_init(smartconfig_type_t type);

#endif // !__BSP_WIFI_H__