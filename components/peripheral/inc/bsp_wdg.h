#ifndef __BSP_WDG_H__
#define __BSP_WDG_H__

#include "esp_timer.h"
#include "esp_system.h"

void bsp_wdg_init(void);
void bsp_wdg_start(uint64_t period);
void bsp_wdg_feed(void);

#endif // !__BSP_WDG_H__