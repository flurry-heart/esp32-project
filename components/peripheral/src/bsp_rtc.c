#include "bsp_rtc.h"

/**
 * @brief 设置RTC的时间
 * 
 * @param year 年
 * @param month 月
 * @param day 日
 * @param hour 时
 * @param min 分
 * @param sec 秒
 */
void bsp_rtc_set_date_time(int year, int month, int day, int hour, int min, int sec)
{
    time_t total_seconds = 0;
    struct tm date_time = {0};
    struct timeval time_value = {0};

    date_time.tm_year = year - 1900;
    date_time.tm_mon = month - 1;
    date_time.tm_mday = day;
    date_time.tm_hour = hour;
    date_time.tm_min = min;
    date_time.tm_sec = sec;
    // 该值大于零表示当前使用了夏令时，等于零表示当前未使用夏令时，小于零则表示该信息不可用
    date_time.tm_isdst = -1;

    // 获取自1970年1月1日0时0分0秒（UTC/GMT 时间）到当前时间的总秒数
    total_seconds = mktime(&date_time);

    time_value.tv_sec = total_seconds;
    // 设置当前时间
    settimeofday(&time_value, NULL);
}

/**
 * @brief 获取RTC的时间
 * 
 * @param calendar 时间结构体指针
 * @return calendar_t 时间结构体
 */
calendar_t bsp_rtc_get_date_time(calendar_t *calendar)
{
    struct tm *date_time;
    time_t total_seconds;

    // 返回自(1970.1.1 00:00:00 UTC)经过的时间(秒)
    time(&total_seconds);
    // 将时间戳转换为tm结构体
    date_time = localtime(&total_seconds);

    if (calendar == NULL)
    {
        calendar_t temp_calendar = 
        {
            .year = date_time->tm_year + 1900,
            .month = date_time->tm_mon + 1,
            .date = date_time->tm_mday,
            .hour = date_time->tm_hour,
            .min = date_time->tm_min,
            .sec = date_time->tm_sec,
            .week = date_time->tm_wday,
        };

        return temp_calendar;
    }

    calendar->year = date_time->tm_year + 1900;
    calendar->month = date_time->tm_mon + 1;
    calendar->date = date_time->tm_mday;
    calendar->hour = date_time->tm_hour;
    calendar->min = date_time->tm_min;
    calendar->sec = date_time->tm_sec;
    calendar->week = date_time->tm_wday;

    return *calendar;
}