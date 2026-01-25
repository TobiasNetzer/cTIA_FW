/*
 * config_storage.h
 *
 *  Created on: Jan 25, 2026
 *      Author: tobias
 */

#ifndef INC_CONFIG_STORAGE_H_
#define INC_CONFIG_STORAGE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Address of the last Flash page (512 KB device, 2 KB pages) */
#define CONFIG_FLASH_PAGE_ADDR   0x0807F800U

#define CONFIG_VERSION           1
#define CONFIG_VALID_MARKER      0x0000000000C0FFEEULL

/**
 * @brief  Loads configuration data from Flash.
 *
 * Reads the version field, configuration payload, and validity marker
 * from the dedicated Flash page. If the version matches and the marker
 * is valid, the configuration is copied into @p data.
 *
 * @param[out] data       Pointer to the destination buffer.
 * @param[in]  data_size  Size of the configuration structure.
 *
 * @retval true   Configuration loaded successfully.
 * @retval false  Flash content invalid or version mismatch.
 */
bool config_storage_load(void *data, size_t data_size);

/**
 * @brief  Saves configuration data to Flash.
 *
 * Erases the dedicated Flash page, writes the version field,
 * the configuration payload (padded to 8‑byte boundaries),
 * and finally the validity marker.
 *
 * @param[in] data        Pointer to the configuration structure.
 * @param[in] data_size   Size of the configuration structure.
 *
 * @retval true   Configuration saved successfully.
 * @retval false  Flash erase or write failed.
 */
bool config_storage_save(const void *data, size_t data_size);

#endif /* INC_CONFIG_STORAGE_H_ */
