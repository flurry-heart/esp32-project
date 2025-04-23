#ifndef __SDCARD_H__
#define __SDCARD_H__

#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_mac.h"

#include "bsp_spi.h"

#define SD_MOUNT_POING      "/sdcard"

extern sdmmc_card_t *g_sdmmc_card;
extern spi_device_handle_t g_sdcard_spi_device_handle;

void sdcard_init(sdmmc_card_t **card, spi_host_device_t host_id, gpio_num_t sdcard_cs_gpio_num, char *mount_point, bool format_if_mount_failed);

#endif // !__SDCARD_H__