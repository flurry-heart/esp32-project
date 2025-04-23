#include "sdcard.h"

sdmmc_card_t *g_sdmmc_card;
spi_device_handle_t g_sdcard_spi_device_handle;

void sdcard_init(sdmmc_card_t **card, spi_host_device_t host_id, gpio_num_t sdcard_cs_gpio_num, char *mount_point, bool format_if_mount_failed) 
{
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_device_config_t sdspi_device_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {0};

    // SD卡参数配置
    host.slot = host_id;                                                        // 使用SPI端口

    // SD卡引脚配置
    sdspi_device_config.host_id   = host_id;
    sdspi_device_config.gpio_cs   = sdcard_cs_gpio_num;
    sdspi_device_config.gpio_cd   = SDSPI_SLOT_NO_CD;
    sdspi_device_config.gpio_wp   = SDSPI_SLOT_NO_WP;
    sdspi_device_config.gpio_int  = SDSPI_SLOT_NO_INT;

    // 文件系统挂载配置
    mount_config.format_if_mount_failed = format_if_mount_failed;               // 如果挂载失败：true会重新分区和格式化/false不会重新分区和格式化
    mount_config.max_files = 5;                                                 // 打开文件最大数量
    mount_config.allocation_unit_size = 16 * 1024;                              // 硬盘分区簇的大小

    esp_vfs_fat_sdspi_mount(mount_point, &host, &sdspi_device_config, &mount_config, card);
}