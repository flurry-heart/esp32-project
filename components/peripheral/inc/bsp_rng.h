#ifndef __BSP_RNG_H__
#define __BSP_RNG_H__

#include "esp_random.h"

uint32_t bsp_rng_get_random_number(void);
uint32_t bsp_rng_get_random_number_in_range(uint32_t min, uint32_t max);

#endif // !__BSP_RNG_H__