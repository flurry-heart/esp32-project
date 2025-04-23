#include "bsp_rng.h"

/**
 * @brief 获取0到UINT32_MAX之间的随机值
 * 
 * @return uint32_t 获取的随机数
 */
uint32_t bsp_rng_get_random_number(void)
{
    uint32_t random_number = esp_random();
    return random_number;
}

/**
 * @brief 获取min到max之间的随机值
 * 
 * @param min 最小值
 * @param max 最大值
 * @return uint32_t 获取的随机数
 */
uint32_t bsp_rng_get_random_number_in_range(uint32_t min, uint32_t max)
{
    uint32_t random_number = bsp_rng_get_random_number();
    random_number = random_number % (max - min + 1) + min;
    return random_number;
}