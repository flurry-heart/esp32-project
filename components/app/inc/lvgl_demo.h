#ifndef __LVGL_DEMO_H__
#define __LVGL_DEMO_H__

#include "freertos/FreeRTOS.h"

#include "esp_timer.h"

#include "lvgl.h"
#include "demos/lv_demos.h"

#include "lcd/lcd.h"
#include "encoder/rotary_encoder.h"

#define MY_DISP_HOR_RES     128
#define MY_DISP_VER_RES     160

#define BYTE_PER_PIXEL      (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */

void lvgl_demo(void);

#endif // !__LVGL_DEMO_H__