/*
 * device_config.h
 *
 *  Created on: Jan 25, 2026
 *      Author: tobias
 */

#ifndef INC_DEVICE_CONFIG_H_
#define INC_DEVICE_CONFIG_H_

#include <stdint.h>

typedef struct __attribute__((aligned(8))) device_config_s {
    uint32_t serial_number;
    uint8_t  meas_ch_count;
    uint8_t  stim_ch_count;
    uint8_t  ext_stim_ch_count;

    uint8_t  is_available_uart;
    uint8_t  is_available_i2c;
    uint8_t  is_available_rs485;

    uint8_t  crc8;
} device_config_t;

void device_config_load(device_config_t* cfg);
void device_config_save(const device_config_t* cfg);
void device_config_defaults(device_config_t* cfg);

#endif /* INC_DEVICE_CONFIG_H_ */
