#include "bsp_mcpwm.h"

bool g_mcpwm_capture_finsh_status = false;

mcpwm_cap_timer_handle_t g_mcpwm_capture_timer_handle;
uint32_t g_mcpwm_capture_ticks;

/**
 * @brief MCPWM输入捕获初始化
 * 
 * @param group_id MCPWM组ID
 * @param gpio_num 捕获引脚
 * @param rising_edge 是否捕获上升沿
 * @param falling_edge 是否捕获下降沿
 * @param idle_level 上拉/下拉
 * @param timer_handle 捕获定时器句柄
 * @param mcpwm_capture_event_callback 捕获回调函数
 */
void bsp_mcpwm_input_capture_init(mcpwm_cap_timer_handle_t *timer_handle, int group_id, int gpio_num, uint32_t prescale, bool rising_edge, bool falling_edge, int idle_level, mcpwm_capture_event_cb_t mcpwm_capture_event_callback)
{
    mcpwm_capture_timer_config_t mcpwm_capture_timer_config = {0};
    mcpwm_capture_channel_config_t mcpwm_capture_channel_config = {0};
    mcpwm_cap_channel_handle_t mcpwm_capture_channel_handle = NULL;
    mcpwm_capture_event_callbacks_t mcpwm_capture_event_callbacks = {0};

    // 创建MCPWM捕获定时器
    mcpwm_capture_timer_config.group_id = group_id;                             // 指定从哪个组分配捕获计时器
    mcpwm_capture_timer_config.clk_src = MCPWM_CAPTURE_CLK_SRC_DEFAULT;         // 捕获定时器时钟源
    mcpwm_new_capture_timer(&mcpwm_capture_timer_config, timer_handle);

    // 初始化捕获通道
    mcpwm_capture_channel_config.gpio_num = gpio_num;                           // 捕获引脚
    mcpwm_capture_channel_config.prescale = prescale;                           // 分频系数
    mcpwm_capture_channel_config.flags.pos_edge = rising_edge;                  // 是否捕获上升沿
    mcpwm_capture_channel_config.flags.neg_edge = falling_edge;                 // 是否捕获下降沿
    if (idle_level)
    {
        mcpwm_capture_channel_config.flags.pull_up = true;                      // 是否使能上拉
        mcpwm_capture_channel_config.flags.pull_down = false;                   // 是否使能下拉
    }
    else
    {
        mcpwm_capture_channel_config.flags.pull_up = false;                     // 是否使能上拉
        mcpwm_capture_channel_config.flags.pull_down = true;                    // 是否使能下拉
    }
    mcpwm_new_capture_channel(*timer_handle, &mcpwm_capture_channel_config, &mcpwm_capture_channel_handle);

    // 注册捕获事件回调函数
    mcpwm_capture_event_callbacks.on_cap = mcpwm_capture_event_callback;
    mcpwm_capture_channel_register_event_callbacks(mcpwm_capture_channel_handle, &mcpwm_capture_event_callbacks, NULL);

    // 使能捕获通道
    mcpwm_capture_channel_enable(mcpwm_capture_channel_handle);

    // 使能定时器
    mcpwm_capture_timer_enable(*timer_handle);
}

/**
 * @brief 捕获事件回调函数
 * 
 * @param cap_channel 捕获通道
 * @param edata 事件数据
 * @param user_ctx 用户的数据，将直接传递给回调函数
 * @return true 有高优先级的任务被唤醒
 * @return false 没有高优先级的任务被唤醒
 */
bool mcpwm_capture_envent_callback(mcpwm_cap_channel_handle_t cap_channel, const mcpwm_capture_event_data_t *edata, void *user_data)
{
    static uint32_t rising_edge_capture_value = 0;
    static uint32_t falling_edge_capture_value = 0;

    if (!g_mcpwm_capture_finsh_status)
    {
        // 如果捕获的是上升沿
        if (edata->cap_edge == MCPWM_CAP_EDGE_POS)
        {
            rising_edge_capture_value = edata->cap_value;
            falling_edge_capture_value = rising_edge_capture_value;
        }
        // 如果捕获的是下降沿
        else if (edata->cap_edge == MCPWM_CAP_EDGE_NEG)
        {
            falling_edge_capture_value = edata->cap_value;
            // 计算高电平脉冲时的计数值
            g_mcpwm_capture_ticks = falling_edge_capture_value - rising_edge_capture_value;
            g_mcpwm_capture_finsh_status = true;
        }
    }

    return true;
}