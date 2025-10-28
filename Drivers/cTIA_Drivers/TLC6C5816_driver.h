/*
 * TLC6C5816_driver.h
 *
 *  Created on: Oct 21, 2025
 *      Author: tobias
 */

/*
 * The MISO Line (SER_OUT) is not connected, since MOSI feeds SER_IN of 3 TLC6C5816.
 * The SER_OUT is a push-pull driver, so parallel connection is not possible without additional circuitry.
 * Another approach would be to connect all the TLC6C5816 in a dasy-chain configuration,
 * however this would mean that all the drivers need to be populated, even if for example only 16 MEAS_CH are needed.
 *
 * In the current HW revision, reading of the fault registers is not possible!
 */

#ifndef CTIA_DRIVERS_TLC6C5816_DRIVER_H_
#define CTIA_DRIVERS_TLC6C5816_DRIVER_H_

#include <stdint.h>
#include "stm32g0xx_hal.h"

#define MAX_CH_COUNT 32 // Has to be a multiple of 16, code will break otherwise!
#define MAX_CH_PER_CHIP 16
#define BYTES_PER_CHIP 3
#define SPI_FRAME_SIZE (BYTES_PER_CHIP * (MAX_CH_COUNT / MAX_CH_PER_CHIP))

HAL_StatusTypeDef tlc6c5816_set_exclusive_output_channel(uint8_t channel);

HAL_StatusTypeDef tlc6c5816_set_output_channel_bitfield(const uint8_t *payload, uint8_t size);

#endif /* CTIA_DRIVERS_TLC6C5816_DRIVER_H_ */
