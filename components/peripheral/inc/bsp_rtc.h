#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

#include <stdint.h>
#include <sys/time.h>

/* 时间结构体, 包括年月日周时分秒等信息 */
typedef struct Calendar_t
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;

    uint16_t year;
    uint8_t month;
    uint8_t date;
    uint8_t week;
}calendar_t;

void bsp_rtc_set_date_time(int year, int month, int day, int hour, int min, int sec);
calendar_t bsp_rtc_get_date_time(calendar_t *calendar);

#endif // !__BSP_RTC_H__