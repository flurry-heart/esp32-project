#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "esp_timer.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"

extern esp_timer_handle_t g_esp_timer_handle;
extern gptimer_handle_t g_gptimer_handle;

void bsp_esp_timer_init(esp_timer_handle_t *handle);
void bsp_gptimer_init(gptimer_handle_t *handle, uint32_t resolution, uint64_t alarm_count, gptimer_alarm_cb_t gptimer_alarm_cbbs);
bool gptimer_alarm_callback(gptimer_handle_t handle, const gptimer_alarm_event_data_t *edata, void *user_data);

#endif // !__BSP_TIMER_H__