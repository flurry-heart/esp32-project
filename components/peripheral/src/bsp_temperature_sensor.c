#include "bsp_temperature_sensor.h"

temperature_sensor_handle_t g_temperature_sensor_handle;

/**
 * @brief 初始化内部温度传感器
 * 
 * @param handle 内部温度传感器句柄
 * @param min 温度范围的最小值
 * @param max 温度范围的最大值
 */
void bsp_temperature_sensor_init(temperature_sensor_handle_t *handle, int min, int max)
{
    temperature_sensor_config_t temperature_sensor_config = {0};

    temperature_sensor_config.clk_src = TEMPERATURE_SENSOR_CLK_SRC_DEFAULT;     // 时钟源
    temperature_sensor_config.range_min = min;                                  // 温度范围的最小值
    temperature_sensor_config.range_max = max;                                  // 温度范围的最大值
    temperature_sensor_install(&temperature_sensor_config, handle);
}

/**
 * @brief 获取内部温度传感器的当前数据
 * 
 * @param handle 内部温度传感器句柄
 * @return float 内部温度传感器的当前数据
 */
float bsp_temperature_sensor_get_temperature(temperature_sensor_handle_t handle)
{
    float temperature = 0;

    temperature_sensor_enable(handle);                                          // 使能温度传感器
    temperature_sensor_get_celsius(handle, &temperature);                       // 获取传输的传感器数据
    temperature_sensor_disable(handle);                                         // 禁用温度传感器，节约功耗

    return temperature;
}