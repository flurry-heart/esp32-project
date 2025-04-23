#ifndef __BSP_MCPWM_H__
#define __BSP_MCPWM_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"

extern mcpwm_cap_timer_handle_t g_mcpwm_capture_timer_handle;
extern bool g_mcpwm_capture_finsh_status;
extern uint32_t g_mcpwm_capture_ticks;

void bsp_mcpwm_input_capture_init(mcpwm_cap_timer_handle_t *timer_handle, int group_id, int gpio_num, uint32_t prescale, bool rising_edge, bool falling_edge, int idle_level, mcpwm_capture_event_cb_t mcpwm_capture_event_callback);
bool mcpwm_capture_envent_callback(mcpwm_cap_channel_handle_t cap_channel, const mcpwm_capture_event_data_t *edata, void *user_data);

#endif // !__BSP_MCPWM_H__