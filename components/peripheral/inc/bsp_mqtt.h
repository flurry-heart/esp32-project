#ifndef __BSP_MQTT_H__
#define __BSP_MQTT_H__

#include "mqtt_client.h"

// MQTT的端口是1883，MQTTS的端口是8883
#define MQTT_DEVICE_ID          "67fa5ad42902516e8670ba2e_D001"                                         // 设备ID
#define MQTT_DEVICE_SECRET      " "                                      // 设备密钥

#define MQTT_HOSTNAME           "a0278c2159.st1.iotda-device.cn-north-4.myhuaweicloud.com"              // MQTT服务器的域名
#define MQTT_PORT               1883                                                                    // MQTT服务器的端口号

#define MQTT_CLIENT_USERNAME    "67fa5ad42902516e8670ba2e_D001"                                         // 客户端用户名
#define MQTT_CLIENT_PASSWORD    "0fc313506e599821d3a72334d8c8c89f2f019130efc270d53e71fc8b149850ba"      // 客户端密码
#define MQTT_CLIENT_ID          "67fa5ad42902516e8670ba2e_D001_0_0_2025041214"                          // 客户端ID

#define MQTT_DEVICE_REPORT_PROPERTIES_TOPIC     "$oc/devices/"MQTT_DEVICE_ID"/sys/properties/report"    // 设备上报属性报文

extern bool g_mqtt_connect_flag;                                                // MQTT连接标志位

extern esp_mqtt_client_handle_t g_mqtt_client_handle;                           // MQTT客户端句柄

void bsp_mqtt_client_init(void);

#endif // !__BSP_MQTT_H__