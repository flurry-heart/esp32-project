#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "esp_adc/adc_oneshot.h"

extern adc_oneshot_unit_handle_t g_adc_oneshot_unit_handle;

void bsp_adc_oneshot_init(adc_oneshot_unit_handle_t *handle, adc_unit_t unit, adc_channel_t channel);
int bsp_adc_oneshot_read(adc_oneshot_unit_handle_t handle, adc_channel_t adc_channel);

#endif // !__BSP_ADC_H__