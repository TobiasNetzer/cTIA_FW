/*
 * device_config.c
 *
 *  Created on: Jan 25, 2026
 *      Author: tobias
 */

#include "device_config.h"
#include "config_storage.h"

void device_config_defaults(device_config_t* cfg)
{
    cfg->serial_number = 0;
    cfg->stim_ch_count = 0;
    cfg->meas_ch_count = 0;
    cfg->ext_stim_ch_count = 0;

    cfg->is_available_uart = 1;
    cfg->is_available_i2c = 1;
    cfg->is_available_rs485 = 1;
}

void device_config_load(device_config_t* cfg)
{
    if (!config_storage_load(cfg, sizeof(device_config_t))) {
        device_config_defaults(cfg);
        device_config_save(cfg);
    }
}

void device_config_save(const device_config_t* cfg)
{
    config_storage_save(cfg, sizeof(device_config_t));
}
