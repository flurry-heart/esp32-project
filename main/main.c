#include "freertos/FreeRTOS.h"

#include "lvgl_demo.h"

#include "lcd.h"
#include "rotary_encoder.h"

// app_main()函数是ESP32的入口函数，它是FreRTOS的一个任务，任务优先级是1
// main()函数是C语言入口函数，它会在编译过程中插入到二进制文件中的
void app_main(void)
{
    lvgl_demo();

    while (1)
    {
        // 将一个任务延迟给定的滴答数，IDF中提供pdMS_TO_TICKS可以将指定的ms转换为对应的tick数
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}