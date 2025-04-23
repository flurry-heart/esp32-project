#include "bsp_timer.h"

esp_timer_handle_t g_esp_timer_handle;
gptimer_handle_t g_gptimer_handle;

static void esp_timer_callback(void *arg);

/**
 * @brief 系统定时器初始化
 * 
 * @param handle 系统定时器句柄
 */
void bsp_esp_timer_init(esp_timer_handle_t *handle)
{
    esp_timer_create_args_t esp_timer_create_args = {0};

    esp_timer_create_args.callback = esp_timer_callback;
    esp_timer_create_args.arg = NULL;
    esp_timer_create(&esp_timer_create_args, handle);
}

/**
 * @brief 系统定时器回调函数
 * 
 * @param arg 传入系统定时器回调函数的参数
 */
static void esp_timer_callback(void *arg)
{
    gpio_set_level(GPIO_NUM_0, !gpio_get_level(GPIO_NUM_0));
}

/**
 * @brief 通用定时器初始化
 * 
 * @param handle 通用定时器句柄
 * @param resolution 定时器分辨率
 * @param alarm_count 报警目标计数值
 * @param gptimer_alarm_cbbs 报警回调函数
 * 
 * @note 计数器每滴答一次相当于 1 / resolution 秒，IDF文档要求该值 2 < clk / resolution < 65536
 */
void bsp_gptimer_init(gptimer_handle_t *handle, uint32_t resolution, uint64_t alarm_count, gptimer_alarm_cb_t gptimer_alarm_cbbs)
{
    gptimer_config_t gptimer_config = {0};
    gptimer_alarm_config_t gptimer_alarm_config = {0};
    gptimer_event_callbacks_t gptimer_event_callback = {0};

    // 配置通用定时器
    gptimer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;                           // 选择定时器时钟源
    gptimer_config.direction = GPTIMER_COUNT_UP;                                // 递增计数模式
    gptimer_config.resolution_hz = resolution;                                  // 定时器分辨率
    gptimer_new_timer(&gptimer_config, handle);                                 // 创建新的通用定时器，并返回句柄
    
    // 设置原始计数值
    gptimer_set_raw_count(*handle, alarm_count);

    gptimer_alarm_config.alarm_count = alarm_count;                             // 报警目标计数值
    gptimer_alarm_config.flags.auto_reload_on_alarm = true;                     // 当报警发生时，是否自动重载
    gptimer_set_alarm_action(*handle, &gptimer_alarm_config);                   // 设置报警动作

    // 设置报警回调函数
    gptimer_event_callback.on_alarm = gptimer_alarm_cbbs;
    gptimer_register_event_callbacks(*handle, &gptimer_event_callback, NULL);   // 注册事件回调函数

    gptimer_enable(g_gptimer_handle);                                           // 使能定时器
}

/**
 * @brief 定时器报警回调函数
 * 
 * @param handle 定时器句柄
 * @param edata 报警事件数据
 * @param user_data 用户传递给报警回调函数的参数
 * @return true 有高优先级的任务被唤醒
 * @return false 没有高优先级的任务被唤醒
 */
bool gptimer_alarm_callback(gptimer_handle_t handle, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    gpio_set_level(GPIO_NUM_0, !gpio_get_level(GPIO_NUM_0));

    return false;
}