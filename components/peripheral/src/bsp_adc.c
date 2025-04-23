#include "bsp_adc.h"

adc_oneshot_unit_handle_t g_adc_oneshot_unit_handle;

/**
 * @brief ADC单次转换初始化函数
 * 
 * @param handle ADC句柄
 * @param unit 使用的ADC单元
 * @param channel 使用的ADC通道
 */
void bsp_adc_oneshot_init(adc_oneshot_unit_handle_t *handle, adc_unit_t unit, adc_channel_t channel)
{
    adc_oneshot_unit_init_cfg_t adc_oneshot_unit_init_config = {0};
    adc_oneshot_chan_cfg_t adc_oneshot_channel_config = {0};

    adc_oneshot_unit_init_config.unit_id = unit;                                // 使用的ADC单元
    adc_oneshot_unit_init_config.clk_src = ADC_RTC_CLK_SRC_DEFAULT;             // ADC的时钟源
    adc_oneshot_new_unit(&adc_oneshot_unit_init_config, handle);

    adc_oneshot_channel_config.atten = ADC_ATTEN_DB_12;                         // 衰减系数
    adc_oneshot_channel_config.bitwidth = ADC_BITWIDTH_12;                      // ADC的位数
    adc_oneshot_config_channel(*handle, channel, &adc_oneshot_channel_config);
}

/**
 * @brief ADC单次读取原始数据
 * 
 * @param handle ADC句柄
 * @param adc_channel 使用的ADC通道
 * @return int 读取的原始数据
 */
int bsp_adc_oneshot_read(adc_oneshot_unit_handle_t handle, adc_channel_t adc_channel)
{
    int raw_data = 0;
    esp_err_t result = ESP_OK;
    result = adc_oneshot_read(handle, adc_channel, &raw_data);
    
    return result == ESP_OK ? raw_data : -1;
}