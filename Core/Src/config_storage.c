/*
 * config_storage.c
 *
 *  Created on: Jan 25, 2026
 *      Author: tobias
 */
#include "config_storage.h"
#include "stm32g0xx_hal.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define CONFIG_VALID_MARKER   0x0000000000C0FFEEULL

bool config_storage_load(void *data, size_t data_size)
{
    if (data == NULL || data_size == 0)
        return false;

    uint8_t *flash_ptr = (uint8_t *)CONFIG_FLASH_PAGE_ADDR;

    uint32_t stored_version = *(uint32_t *)flash_ptr;
    if (stored_version != CONFIG_VERSION)
        return false;

    flash_ptr += 8;

    memcpy(data, flash_ptr, data_size);

    uint64_t stored_marker = *(uint64_t *)(flash_ptr + data_size);

    return (stored_marker == CONFIG_VALID_MARKER);
}

bool config_storage_save(const void *data, size_t data_size)
{
    if (data == NULL || data_size == 0)
        return false;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase = {
        .TypeErase = FLASH_TYPEERASE_PAGES,
        .Page      = (CONFIG_FLASH_PAGE_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE,
        .NbPages   = 1
    };

    uint32_t page_error = 0;
    if (HAL_FLASHEx_Erase(&erase, &page_error) != HAL_OK || page_error != 0xFFFFFFFFU) {
        HAL_FLASH_Lock();
        return false;
    }

    uint32_t addr = CONFIG_FLASH_PAGE_ADDR;

    uint64_t version64 = (uint64_t)CONFIG_VERSION;
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, version64) != HAL_OK) {
        HAL_FLASH_Lock();
        return false;
    }
    addr += 8;

    const uint8_t *src = (const uint8_t *)data;
    size_t remaining = data_size;

    while (remaining > 0) {
        uint8_t buf[8] = {0};
        size_t chunk = (remaining > 8U) ? 8U : remaining;

        memcpy(buf, src, chunk);

        uint64_t dw = 0;
        memcpy(&dw, buf, 8);

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, dw) != HAL_OK) {
            HAL_FLASH_Lock();
            return false;
        }

        addr      += 8;
        src       += chunk;
        remaining -= chunk;
    }

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, CONFIG_VALID_MARKER) != HAL_OK) {
        HAL_FLASH_Lock();
        return false;
    }

    HAL_FLASH_Lock();
    return true;
}
