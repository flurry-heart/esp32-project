#ifndef __BSP_TEMPERATURE_SENSOR_H__
#define __BSP_TEMPERATURE_SENSOR_H__

#include "driver/temperature_sensor.h"

extern temperature_sensor_handle_t g_temperature_sensor_handle;

void bsp_temperature_sensor_init(temperature_sensor_handle_t *handle, int min, int max);
float bsp_temperature_sensor_get_temperature(temperature_sensor_handle_t handle);

#endif // !__BSP_TEMPERATURE_SENSOR_H__