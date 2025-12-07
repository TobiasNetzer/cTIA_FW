/*
 * TLC6C5816_driver.c
 *
 *  Created on: Oct 21, 2025
 *      Author: tobias
 */

#include "TLC6C5816_driver.h"
#include "spi.h"

HAL_StatusTypeDef tlc6c5816_set_exclusive_output_channel(uint8_t channel) {

	if (channel > MAX_CH_COUNT || channel == 0) return HAL_ERROR;

	channel--; // 0-based index

	uint8_t shift_reg_data[SPI_FRAME_SIZE] = {0};

    uint8_t chip_index = !(channel / MAX_CH_PER_CHIP); // 16 CH per chip (invert since byte order is MSByte first)
    uint8_t chip_channel = channel % MAX_CH_PER_CHIP;  // find which channel (2 per chip)
    uint8_t data_byte_index = !(chip_channel / 8);  // 0 or 1 -> 16 CH stored in 2 bytes, check if bit is located in the High or Low-Byte (invert since byte order is MSByte first)
    uint8_t bit_index = chip_channel % 8;

    uint8_t byte_index = (chip_index * BYTES_PER_CHIP) + 1 + data_byte_index;

    shift_reg_data[byte_index] = (1 << bit_index);

    HAL_GPIO_WritePin(SHIFT_REG_CLR_GPIO_Port, SHIFT_REG_CLR_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(SHIFT_REG_CLR_GPIO_Port, SHIFT_REG_CLR_Pin, GPIO_PIN_SET);

    if (HAL_SPI_Transmit(&hspi1, shift_reg_data, SPI_FRAME_SIZE, 100) != HAL_OK) return HAL_ERROR;

    return HAL_OK;
}

HAL_StatusTypeDef tlc6c5816_set_output_channel_bitfield(const uint8_t *bitfield, uint8_t size) {

	uint8_t shift_reg_data[SPI_FRAME_SIZE] = {0};
	uint8_t index = SPI_FRAME_SIZE;
	uint8_t bitfield_index = 0;

	HAL_GPIO_WritePin(SHIFT_REG_CLR_GPIO_Port, SHIFT_REG_CLR_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(SHIFT_REG_CLR_GPIO_Port, SHIFT_REG_CLR_Pin, GPIO_PIN_SET);

	if (size > (MAX_CH_COUNT / 8) || size == 0) return HAL_ERROR;
	if (!bitfield) return HAL_ERROR;

	if (size % 2 != 0) {
		shift_reg_data[2] = bitfield[0];
		index += 3;
		bitfield_index += 1;
	}

	while (bitfield_index <= (size / 2)) {
		/* DIAG mode is not required, so DIAG bits remain 0x00 */
		shift_reg_data[index - 2] = bitfield[bitfield_index + 1];
		shift_reg_data[index - 1] = bitfield[bitfield_index];
		index -= 3;
		bitfield_index += 2;
	}

	if (HAL_SPI_Transmit(&hspi1, shift_reg_data, SPI_FRAME_SIZE, 100) != HAL_OK) return HAL_ERROR;

	return HAL_OK;
}
