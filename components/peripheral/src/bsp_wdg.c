#include "bsp_wdg.h"

static esp_timer_handle_t g_wdg_esp_timer_handle;
static uint64_t g_max_wdg_feed_time;

static void wdg_callback(void *arg);

/**
 * @brief 看门狗初始化
 * 
 * @param handle 系统定时器句柄
 */
void bsp_wdg_init(void)
{
    esp_timer_create_args_t esp_timer_create_args = {0};

    esp_timer_create_args.callback = wdg_callback;
    esp_timer_create_args.arg = NULL;
    esp_timer_create(&esp_timer_create_args, &g_wdg_esp_timer_handle);
}

/**
 * @brief 启动看门狗
 * 
 * @param period 喂狗的超时时间，单位是微秒
 */
void bsp_wdg_start(uint64_t period)
{
    g_max_wdg_feed_time = period;
    esp_timer_start_periodic(g_wdg_esp_timer_handle, period);                   // 每周期内触发一次
}

/**
 * @brief 喂狗
 * 
 */
void bsp_wdg_feed(void)
{
    // 重新启动当前运行的计时器，用以模拟喂狗过程
    esp_timer_restart(g_wdg_esp_timer_handle, g_max_wdg_feed_time);
}

/**
 * @brief 系统定时器回调函数
 * 
 * @param arg 传入系统定时器回调函数的参数
 */
static void wdg_callback(void *arg)
{
    // 若没有及时进行喂狗，那么芯片将一直进行复位
    esp_restart();
}
