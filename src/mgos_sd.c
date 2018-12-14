#include "mgos.h"

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/dirent.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

#include "mgos_sd.h"

bool mgos_sd_init(void)
{
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    //  host.slot = VSPI_HOST;
    //  host.flags = SDMMC_HOST_FLAG_SPI;
    //  host.max_freq_khz = 400;
    //  host.max_freq_khz = 5000;
    //  host.slot         = HSPI_HOST;

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = mgos_sys_config_get_sd_spi_pin_miso(); // PIN_NUM_MISO;
    slot_config.gpio_mosi = mgos_sys_config_get_sd_spi_pin_mosi(); // PIN_NUM_MOSI;
    slot_config.gpio_sck = mgos_sys_config_get_sd_spi_pin_clk();   // PIN_NUM_CLK;
    slot_config.gpio_cs = mgos_sys_config_get_sd_spi_pin_cs();     // PIN_NUM_CS;

    //  slot_config.dma_channel = 0;
    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    // max_files Max number of open files
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5};

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    sdmmc_card_t *card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set format_if_mount_failed = true.");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%d). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     ret);
        }
        return 0;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
    return 1;
}