/*
 * cTIA.c
 *
 *  Created on: Oct 19, 2025
 *      Author: tobias
 */

#include <cTIA.h>
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "device_config.h"
#include "i2c.h"

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c2;
extern device_config_t cTIA_config;

static ctia_state_t ctia_state = {0};
static uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];

static inline uint32_t dma_rx_pos(void) {
    return UART_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
}

static ctia_status_t cTIA_uart_configure(uint32_t baud, uint8_t data_bits, uint8_t stop_bits, uint8_t parity) {

	HAL_UART_DMAStop(&huart1);
	huart1.RxState = HAL_UART_STATE_READY;

	if (HAL_UART_DeInit(&huart1) != HAL_OK)
		return CTIA_FAIL;

	huart1.Init.BaudRate = baud;

	switch (data_bits) {
	    case 7:
	        if (parity == UART_PARITY_NONE)
	            huart1.Init.WordLength = UART_WORDLENGTH_7B;
	        else
	            huart1.Init.WordLength = UART_WORDLENGTH_8B;   // 7 data + parity
	        break;

	    case 8:
	        if (parity == UART_PARITY_NONE)
	            huart1.Init.WordLength = UART_WORDLENGTH_8B;
	        else
	            huart1.Init.WordLength = UART_WORDLENGTH_9B;   // 8 data + parity
	        break;

	    default:
	        return CTIA_INVALID_PARAMETER;
	}

	switch (stop_bits) {
		case 1: huart1.Init.StopBits = UART_STOPBITS_1; break;
		case 2: huart1.Init.StopBits = UART_STOPBITS_2; break;
		default: return CTIA_INVALID_PARAMETER;
	}

	switch (parity) {
		case 0: huart1.Init.Parity = UART_PARITY_NONE; break;
		case 1: huart1.Init.Parity = UART_PARITY_EVEN; break;
		case 2: huart1.Init.Parity = UART_PARITY_ODD; break;
		default: return CTIA_INVALID_PARAMETER;
	}

	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if (HAL_UART_Init(&huart1) != HAL_OK)
		return CTIA_FAIL;

	huart1.RxState = HAL_UART_STATE_READY;

	if (HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, UART_RX_BUFFER_SIZE) != HAL_OK)
		return CTIA_FAIL;

	return CTIA_SUCCESS;
}


void cTIA_uart_init(void) {
    HAL_UART_Receive_DMA(&huart1, uart_rx_buffer, UART_RX_BUFFER_SIZE);
}


ctia_status_t cTIA_set_exclusive_meas_h_ch(uint8_t channel) {

	if (channel > cTIA_config.meas_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t old_bitfield[sizeof(ctia_state.active_meas_h_ch_bitfield)];
	memcpy(old_bitfield, ctia_state.active_meas_h_ch_bitfield, sizeof(old_bitfield));

	memset(ctia_state.active_meas_h_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_h_ch_bitfield));

	uint8_t index = (channel - 1) / 8;
	ctia_state.active_meas_h_ch_bitfield[index] |= (1 << ((channel - 1) % 8));

	bool unchanged = (memcmp(ctia_state.active_meas_h_ch_bitfield, old_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield)) == 0);

	if(unchanged) return CTIA_SUCCESS;

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	ctia_status_t status = tlc6c5816_set_exclusive_output_channel(channel);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_meas_h_ch(uint8_t channel) {

	if (channel > cTIA_config.meas_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_meas_h_ch_bitfield[index] |= (1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_h_ch_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);

	return status;
}

ctia_status_t cTIA_set_meas_h_ch_bitfield(const uint8_t *payload, uint8_t size) {

	if (payload == NULL || size == 0) return CTIA_INVALID_PARAMETER;

	uint8_t old_bitfield[sizeof(ctia_state.active_meas_h_ch_bitfield)];
	memcpy(old_bitfield, ctia_state.active_meas_h_ch_bitfield, sizeof(old_bitfield));

	memset(ctia_state.active_meas_h_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_h_ch_bitfield));
	memcpy(ctia_state.active_meas_h_ch_bitfield, payload, size);

	uint8_t clear_bitfield[sizeof(ctia_state.active_meas_h_ch_bitfield)];
	for (uint8_t i = 0; i < sizeof(clear_bitfield); i++) {
		clear_bitfield[i] = old_bitfield[i] & ctia_state.active_meas_h_ch_bitfield[i];
	}

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);

	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(clear_bitfield, sizeof(clear_bitfield));

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);

	if (status != CTIA_SUCCESS) return status;

	HAL_Delay(5);

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);

	status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_h_ch_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield));

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);

	return status;
}

ctia_status_t cTIA_set_exclusive_meas_l_ch(uint8_t channel) {

	if (channel > cTIA_config.meas_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t old_bitfield[sizeof(ctia_state.active_meas_l_ch_bitfield)];
	memcpy(old_bitfield, ctia_state.active_meas_l_ch_bitfield, sizeof(old_bitfield));

	memset(ctia_state.active_meas_l_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_l_ch_bitfield));

	uint8_t index = (channel - 1) / 8;
	ctia_state.active_meas_l_ch_bitfield[index] |= (1 << ((channel - 1) % 8));

	bool unchanged = (memcmp(ctia_state.active_meas_l_ch_bitfield, old_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield)) == 0);

	if(unchanged) return CTIA_SUCCESS;

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	ctia_status_t status = tlc6c5816_set_exclusive_output_channel(channel);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_meas_l_ch(uint8_t channel) {

	if (channel > cTIA_config.meas_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_meas_l_ch_bitfield[index] |= (1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_l_ch_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);

	return status;
}

ctia_status_t cTIA_set_meas_l_ch_bitfield(const uint8_t *payload, uint8_t size) {

	if (payload == NULL || size == 0) return CTIA_INVALID_PARAMETER;

	uint8_t old_bitfield[sizeof(ctia_state.active_meas_l_ch_bitfield)];
	memcpy(old_bitfield, ctia_state.active_meas_l_ch_bitfield, sizeof(old_bitfield));

	memset(ctia_state.active_meas_l_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_l_ch_bitfield));
	memcpy(ctia_state.active_meas_l_ch_bitfield, payload, size);

	uint8_t clear_bitfield[sizeof(ctia_state.active_meas_l_ch_bitfield)];
	for (uint8_t i = 0; i < sizeof(clear_bitfield); i++) {
		clear_bitfield[i] = old_bitfield[i] & ctia_state.active_meas_l_ch_bitfield[i];
	}

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);

	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(clear_bitfield, sizeof(clear_bitfield));

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);

	if (status != CTIA_SUCCESS) return status;

	HAL_Delay(5);

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);

	status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_l_ch_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield));

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);

	return status;
}

ctia_status_t cTIA_set_exclusive_stim_ch(uint8_t channel) {

	if (channel > cTIA_config.stim_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t old_bitfield[sizeof(ctia_state.active_stim_ch_bitfield)];
	memcpy(old_bitfield, ctia_state.active_stim_ch_bitfield, sizeof(old_bitfield));

	memset(ctia_state.active_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_stim_ch_bitfield));

	uint8_t index = (channel - 1) / 8;
	ctia_state.active_stim_ch_bitfield[index] |= (1 << ((channel - 1) % 8));

	bool unchanged = (memcmp(ctia_state.active_stim_ch_bitfield, old_bitfield, sizeof(ctia_state.active_stim_ch_bitfield)) == 0);

	if(unchanged) return CTIA_SUCCESS;

	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	ctia_status_t status = tlc6c5816_set_exclusive_output_channel(channel);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_stim_ch(uint8_t channel) {

	if (channel > cTIA_config.stim_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_stim_ch_bitfield[index] |= (1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_stim_ch_bitfield, sizeof(ctia_state.active_stim_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);

	return status;
}

ctia_status_t cTIA_set_stim_ch_bitfield(const uint8_t *payload, uint8_t size)
{
    if (payload == NULL || size == 0) return CTIA_INVALID_PARAMETER;

    uint8_t old_bitfield[sizeof(ctia_state.active_stim_ch_bitfield)];
    memcpy(old_bitfield, ctia_state.active_stim_ch_bitfield, sizeof(old_bitfield));

    memset(ctia_state.active_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_stim_ch_bitfield));
    memcpy(ctia_state.active_stim_ch_bitfield, payload, size);

    uint8_t clear_bitfield[sizeof(ctia_state.active_stim_ch_bitfield)];
    for (uint8_t i = 0; i < sizeof(clear_bitfield); i++) {
        clear_bitfield[i] = old_bitfield[i] & ctia_state.active_stim_ch_bitfield[i];
    }

    HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);

    ctia_status_t status = tlc6c5816_set_output_channel_bitfield(clear_bitfield, sizeof(clear_bitfield));

    HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);

    if (status != CTIA_SUCCESS) return status;

    HAL_Delay(5);

    HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);

    status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_stim_ch_bitfield, sizeof(ctia_state.active_stim_ch_bitfield));

    HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);

    return status;
}


ctia_status_t cTIA_set_ext_probe_in_state(uint8_t state) {

	if (state > 1) return CTIA_INVALID_PARAMETER;
	if (HAL_GPIO_ReadPin(PROBE_DETECT_GPIO_Port, PROBE_DETECT_Pin) && state) return CTIA_UNAVAILABLE;

	ctia_state.ext_probe_in_state = state;

	HAL_GPIO_WritePin(EXT_PROBE_ON_GPIO_Port, EXT_PROBE_ON_Pin, ctia_state.ext_probe_in_state);

	HAL_Delay(5); // Make sure Relay has enough time to switch

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_set_analog_bus_detect_state(uint8_t state) {

	if (state > 1) return CTIA_INVALID_PARAMETER;

	ctia_state.analog_bus_detect_state = state;

	HAL_GPIO_WritePin(ANALOG_BUS_DETECT_ENABLE_GPIO_Port, ANALOG_BUS_DETECT_ENABLE_Pin, ctia_state.analog_bus_detect_state);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_set_ext_stim_ch(uint8_t channel) {

	if (channel > cTIA_config.ext_stim_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_ext_stim_ch_bitfield[index] |= (1 << bit_pos);

	switch (channel) {
		case 1: {
			HAL_GPIO_WritePin(EXT_STIM_1_ON_GPIO_Port, EXT_STIM_1_ON_Pin, GPIO_PIN_SET);
			break;
		}
		case 2: {
			HAL_GPIO_WritePin(EXT_STIM_2_ON_GPIO_Port, EXT_STIM_2_ON_Pin, GPIO_PIN_SET);
			break;
		}
		case 3: {
			HAL_GPIO_WritePin(EXT_STIM_3_ON_GPIO_Port, EXT_STIM_3_ON_Pin, GPIO_PIN_SET);
			break;
		}
		case 4: {
			HAL_GPIO_WritePin(EXT_STIM_4_ON_GPIO_Port, EXT_STIM_4_ON_Pin, GPIO_PIN_SET);
			break;
		}
	}

	HAL_Delay(5); // Make sure Relay has enough time to switch

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_set_ext_stim_ch_bitfield(const uint8_t *payload, uint8_t size) {

	if (size > (1 + (cTIA_config.ext_stim_ch_count / 8)) || size == 0) return CTIA_UNAVAILABLE;
	if (payload == NULL) return CTIA_INVALID_PARAMETER;

	ctia_status_t status;

	memset(ctia_state.active_ext_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_ext_stim_ch_bitfield));

	for (uint8_t byte = 0; byte < size; byte++) {
		for (uint8_t bit = 0; bit < 8; bit++) {
			if (!(payload[byte] & (1 << bit))) {
				status = cTIA_clear_ext_stim_ch(byte * 8 + bit + 1);
				if (status == CTIA_UNAVAILABLE) continue;
				// Since data is transmitted as uint8_t, the return parameter CTIA_UNAVAILABLE is
				// inevitable because we have no way of telling how many bits are "valid".
				//if (status != CTIA_SUCCESS) return status;
			}
		}
	}

	for (uint8_t byte = 0; byte < size; byte++) {
			for (uint8_t bit = 0; bit < 8; bit++) {
				if (payload[byte] & (1 << bit)) {
					status = cTIA_set_ext_stim_ch(byte * 8 + bit + 1);
					if (status != CTIA_SUCCESS) return status;
				}
			}
		}

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_clear_meas_h_ch(uint8_t channel) {

	if (channel > cTIA_config.meas_ch_count || channel == 0) return CTIA_UNAVAILABLE;

		uint8_t index = (channel - 1) / 8;
		uint8_t bit_pos = (channel - 1) % 8;

		ctia_state.active_meas_h_ch_bitfield[index] &= ~(1 << bit_pos);

		HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);
		ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_h_ch_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield));
		HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);

		return status;
}

ctia_status_t cTIA_clear_meas_h(void) {

	memset(ctia_state.active_meas_h_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_h_ch_bitfield));

	HAL_GPIO_WritePin(SHIFT_REG_CLR_H_GPIO_Port, SHIFT_REG_CLR_H_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(SHIFT_REG_CLR_H_GPIO_Port, SHIFT_REG_CLR_H_Pin, GPIO_PIN_SET);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_clear_meas_l_ch(uint8_t channel) {

	if (channel > cTIA_config.meas_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_meas_l_ch_bitfield[index] &= ~(1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_l_ch_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);

	return status;
}

ctia_status_t cTIA_clear_meas_l(void) {

	memset(ctia_state.active_meas_l_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_l_ch_bitfield));

	HAL_GPIO_WritePin(SHIFT_REG_CLR_L_GPIO_Port, SHIFT_REG_CLR_L_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(SHIFT_REG_CLR_L_GPIO_Port, SHIFT_REG_CLR_L_Pin, GPIO_PIN_SET);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_clear_stim_ch(uint8_t channel) {

	if (channel > cTIA_config.stim_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_stim_ch_bitfield[index] &= ~(1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_stim_ch_bitfield, sizeof(ctia_state.active_stim_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);

	return status;
}

ctia_status_t cTIA_clear_stim(void) {

	memset(ctia_state.active_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_stim_ch_bitfield));

	HAL_GPIO_WritePin(SHIFT_REG_CLR_STIM_GPIO_Port, SHIFT_REG_CLR_STIM_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(SHIFT_REG_CLR_STIM_GPIO_Port, SHIFT_REG_CLR_STIM_Pin, GPIO_PIN_SET);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_clear_ext_stim_ch(uint8_t channel) {

	if (channel > cTIA_config.ext_stim_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_ext_stim_ch_bitfield[index] &= ~(1 << bit_pos);

	switch (channel) {
		case 1: {
			HAL_GPIO_WritePin(EXT_STIM_1_ON_GPIO_Port, EXT_STIM_1_ON_Pin, GPIO_PIN_RESET);
			break;
		}
		case 2: {
			HAL_GPIO_WritePin(EXT_STIM_2_ON_GPIO_Port, EXT_STIM_2_ON_Pin, GPIO_PIN_RESET);
			break;
		}
		case 3: {
			HAL_GPIO_WritePin(EXT_STIM_3_ON_GPIO_Port, EXT_STIM_3_ON_Pin, GPIO_PIN_RESET);
			break;
		}
		case 4: {
			HAL_GPIO_WritePin(EXT_STIM_4_ON_GPIO_Port, EXT_STIM_4_ON_Pin, GPIO_PIN_RESET);
			break;
		}
	}

	HAL_Delay(5); // Make sure Relay has enough time to switch

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_clear_ext_stim(void) {

		ctia_status_t status;

		memset(ctia_state.active_ext_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_ext_stim_ch_bitfield));

		for (uint8_t byte = 0; byte < sizeof(ctia_state.active_ext_stim_ch_bitfield); byte++) {
			for (uint8_t bit = 0; bit < 8; bit++) {
				status = cTIA_clear_ext_stim_ch(byte * 8 + bit + 1);
				if (status == CTIA_UNAVAILABLE) continue; // Since data is transmitted as uint8_t, the return parameter CTIA_UNAVAILABLE is inevitable because we have no way of telling how many bits are "valid".
			}
		}

		return CTIA_SUCCESS;
}

ctia_status_t cTIA_clear_all_relays(void) {

	cTIA_clear_stim();
	cTIA_clear_meas_h();
	cTIA_clear_meas_l();
	cTIA_clear_ext_stim();
	cTIA_set_ext_probe_in_state(0);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_device_id(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	uint16_t id = DEVICE_ID;
	memcpy(buffer, &id, sizeof(id));
	*size = sizeof(id);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_device_name(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	const char *device_name_str = DEVICE_NAME_STRING;
	uint8_t len = strlen(device_name_str);

	memcpy(buffer, device_name_str, len);
	*size = (uint8_t)len;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_serial_number(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	memcpy(buffer, &cTIA_config.serial_number, sizeof(uint32_t));
	*size = sizeof(uint32_t);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_fw_build_date(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	const char *date_str = BUILD_DATE;
	uint8_t len = strlen(date_str);

	memcpy(buffer, date_str, len);
	*size = (uint8_t)len;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_fw_build_time(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	const char *time_str = BUILD_TIME;
	uint8_t len = strlen(time_str);

	memcpy(buffer, time_str, len);
	*size = (uint8_t)len;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_fw_version(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	const char *fw_str = FW_VERSION_STRING;
	uint8_t len = strlen(fw_str);

	memcpy(buffer, fw_str, len);
	*size = (uint8_t)len;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_meas_h_bitfield(uint8_t *bitfield, uint8_t *size) {

    if (bitfield == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

    memcpy(bitfield, ctia_state.active_meas_h_ch_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield));

    *size = sizeof(ctia_state.active_meas_h_ch_bitfield);
    return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_meas_l_bitfield(uint8_t *bitfield, uint8_t *size) {

    if (bitfield == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

    memcpy(bitfield, ctia_state.active_meas_l_ch_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield));

    *size = sizeof(ctia_state.active_meas_l_ch_bitfield);
    return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_stim_bitfield(uint8_t *bitfield, uint8_t *size) {

    if (bitfield == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

    memcpy(bitfield, ctia_state.active_stim_ch_bitfield, sizeof(ctia_state.active_stim_ch_bitfield));

    *size = sizeof(ctia_state.active_stim_ch_bitfield);
    return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_ext_stim_bitfield(uint8_t *bitfield, uint8_t *size) {

    if (bitfield == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

    memcpy(bitfield, ctia_state.active_ext_stim_ch_bitfield, sizeof(ctia_state.active_ext_stim_ch_bitfield));

    *size = sizeof(ctia_state.active_ext_stim_ch_bitfield);
    return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_ext_probe_in_state(uint8_t *state) {

	if (state == NULL) return CTIA_INVALID_PARAMETER;

	*state = ctia_state.ext_probe_in_state;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_analog_bus_detect_state(uint8_t *state) {

	if (state == NULL) return CTIA_INVALID_PARAMETER;

	*state = ctia_state.analog_bus_detect_state;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_available_meas_channels(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	buffer[0] = cTIA_config.meas_ch_count;
	*size = 1;


	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_available_stim_channels(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	buffer[0] = cTIA_config.stim_ch_count;
	*size = 1;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_available_ext_stim_channels(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	buffer[0] = cTIA_config.ext_stim_ch_count;
	*size = 1;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_available_i2c_interface(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	buffer[0] = cTIA_config.is_available_i2c;
	*size = 1;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_available_uart_interface(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	buffer[0] = cTIA_config.is_available_uart;
	*size = 1;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_get_available_rs485_interface(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;

	buffer[0] = cTIA_config.is_available_rs485;
	*size = 1;

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_serial_number(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;
	if (*size != sizeof(uint32_t)) return CTIA_INVALID_PARAMETER;

	memcpy(&cTIA_config.serial_number, buffer, sizeof(uint32_t));

	device_config_save(&cTIA_config);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_available_meas_ch(uint8_t channel_count) {

	if (channel_count > MAX_CH_COUNT) return CTIA_INVALID_PARAMETER;

	cTIA_config.meas_ch_count = channel_count;

	device_config_save(&cTIA_config);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_available_stim_ch(uint8_t channel_count) {

	if (channel_count > MAX_CH_COUNT) return CTIA_INVALID_PARAMETER;

	cTIA_config.stim_ch_count = channel_count;

	device_config_save(&cTIA_config);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_available_ext_stim_ch(uint8_t channel_count) {

	if (channel_count > EXT_STIM_CH_MAX_COUNT) return CTIA_INVALID_PARAMETER;

	cTIA_config.ext_stim_ch_count = channel_count;

	device_config_save(&cTIA_config);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_available_i2c_interface(uint8_t is_available) {

	if (is_available > 1) return CTIA_INVALID_PARAMETER;

	cTIA_config.is_available_i2c = is_available;

	device_config_save(&cTIA_config);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_available_uart_interface(uint8_t is_available) {

	if (is_available > 1) return CTIA_INVALID_PARAMETER;

	cTIA_config.is_available_uart = is_available;

	device_config_save(&cTIA_config);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_available_rs485_interface(uint8_t is_available) {

	if (is_available > 1) return CTIA_INVALID_PARAMETER;

	cTIA_config.is_available_rs485 = is_available;

	device_config_save(&cTIA_config);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_i2c_settings(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;
	if (*size != 1) return CTIA_INVALID_PARAMETER;

	uint8_t raw = buffer[0];

	if (raw >= I2C_SPEED_MAX)
		return CTIA_INVALID_PARAMETER;

	I2C2_SetSpeed((I2C_Speed)raw);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_conf_uart_settings(uint8_t *buffer, uint8_t *size) {

	if (buffer == NULL || size == NULL) return CTIA_INVALID_PARAMETER;
	if (*size != 7) return CTIA_INVALID_PARAMETER;

	uint32_t baud =
		((uint32_t)buffer[0]) |
		((uint32_t)buffer[1] << 8) |
		((uint32_t)buffer[2] << 16) |
		((uint32_t)buffer[3] << 24);

	uint8_t data_bits = buffer[4];
	uint8_t stop_bits = buffer[5];
	uint8_t parity    = buffer[6];

	return cTIA_uart_configure(baud, data_bits, stop_bits, parity);
}

ctia_status_t cTIA_execute_selftest(uint8_t *buffer, uint8_t *size){

	if (buffer == NULL || size == NULL) return CTIA_FAIL;

	uint8_t defective_relays_h[MAX_CH_COUNT / 8];
	uint8_t defective_relays_l[MAX_CH_COUNT / 8];
	memset(&defective_relays_h, 0, sizeof(defective_relays_h));
	memset(&defective_relays_l, 0, sizeof(defective_relays_l));

	cTIA_clear_all_relays();
	cTIA_set_analog_bus_detect_state(true);
	HAL_Delay(100);

	if (!HAL_GPIO_ReadPin(ANALOG_BUS_DETECT_INPUT_GPIO_Port, ANALOG_BUS_DETECT_INPUT_Pin))
		return CTIA_FAIL;

	for (uint8_t i = 1; i <= cTIA_config.meas_ch_count; i++) {

		cTIA_clear_meas_l();
		cTIA_set_exclusive_meas_h_ch(i);
		HAL_Delay(20);

		if (!HAL_GPIO_ReadPin(ANALOG_BUS_DETECT_INPUT_GPIO_Port, ANALOG_BUS_DETECT_INPUT_Pin))
			defective_relays_l[(i - 1) / 8] |= (1 << ((i - 1) % 8));

		cTIA_set_exclusive_meas_l_ch(i);
		HAL_Delay(20);

		if (HAL_GPIO_ReadPin(ANALOG_BUS_DETECT_INPUT_GPIO_Port, ANALOG_BUS_DETECT_INPUT_Pin)){
			defective_relays_h[(i - 1) / 8] |= (1 << ((i - 1) % 8));
			defective_relays_l[(i - 1) / 8] |= (1 << ((i - 1) % 8));
		}

		cTIA_clear_meas_h();
		HAL_Delay(20);

		if (!HAL_GPIO_ReadPin(ANALOG_BUS_DETECT_INPUT_GPIO_Port, ANALOG_BUS_DETECT_INPUT_Pin))
			defective_relays_h[(i - 1) / 8] |= (1 << ((i - 1) % 8));

	}

	memcpy(buffer, defective_relays_h, sizeof(defective_relays_h));
	memcpy(buffer + sizeof(defective_relays_h), defective_relays_l, sizeof(defective_relays_l));
	*size = sizeof(defective_relays_h) + sizeof(defective_relays_l);

	cTIA_clear_all_relays();
	cTIA_set_analog_bus_detect_state(false);
	HAL_Delay(100);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_i2c_transmit(uint8_t *buffer, uint8_t *size) {

    if (buffer == NULL || size == NULL)
        return CTIA_INVALID_PARAMETER;

    if (*size < 5)
        return CTIA_INVALID_PARAMETER;

    if (buffer[0] > 127)
    	return CTIA_INVALID_PARAMETER;

    uint16_t device_addr   = (buffer[0] << 1);

    uint32_t timeout =
        ((uint32_t)buffer[1]) |
        ((uint32_t)buffer[2] << 8) |
        ((uint32_t)buffer[3] << 16) |
        ((uint32_t)buffer[4] << 24);

    uint16_t bytes_to_send = *size - 5;

    *size = 0;

    HAL_StatusTypeDef st = HAL_I2C_Master_Transmit(&hi2c2, device_addr, buffer + 5, bytes_to_send, timeout);

    switch (st)
    {
        case HAL_OK:
            return CTIA_SUCCESS;

        case HAL_TIMEOUT:
            return CTIA_TIMEOUT;

        case HAL_BUSY:
            return CTIA_BUSY;

        case HAL_ERROR:
        default:
            return CTIA_FAIL;
    }
}

ctia_status_t cTIA_i2c_receive(uint8_t *buffer, uint8_t *size)
{
    if (buffer == NULL || size == NULL)
        return CTIA_INVALID_PARAMETER;

    if (*size != 6)
        return CTIA_INVALID_PARAMETER;

    if (buffer[0] > 127)
        return CTIA_INVALID_PARAMETER;

    uint16_t device_addr = buffer[0] << 1;

    uint32_t timeout =
        ((uint32_t)buffer[1]) |
        ((uint32_t)buffer[2] << 8) |
        ((uint32_t)buffer[3] << 16) |
        ((uint32_t)buffer[4] << 24);

    uint8_t bytes_to_read = buffer[5];

    *size = bytes_to_read;

    HAL_StatusTypeDef st = HAL_I2C_Master_Receive(&hi2c2, device_addr, buffer, bytes_to_read, timeout);

    if (st != HAL_OK)
    	*size = 0;

    switch (st) {
        case HAL_OK:
            return CTIA_SUCCESS;

        case HAL_TIMEOUT:
            return CTIA_TIMEOUT;

        case HAL_BUSY:
            return CTIA_BUSY;

        case HAL_ERROR:
        default:
            return CTIA_FAIL;
    }
}

ctia_status_t cTIA_uart_transceive(uint8_t *buffer, uint8_t *size)
{
    if (!buffer || !size)
        return CTIA_INVALID_PARAMETER;

    if (buffer[0] > 120)
        return CTIA_INVALID_PARAMETER;

    uint8_t bytes_to_read = buffer[0];

    uint32_t timeout =
        ((uint32_t)buffer[1]) |
        ((uint32_t)buffer[2] << 8) |
        ((uint32_t)buffer[3] << 16) |
        ((uint32_t)buffer[4] << 24);

    uint16_t bytes_to_send = *size - 5;
    *size = 0;

    uint32_t start_tick = HAL_GetTick();
    uint32_t start_pos  = dma_rx_pos();

    if (bytes_to_send > 0) {
    	HAL_StatusTypeDef st = HAL_UART_Transmit(&huart1, buffer + 5, bytes_to_send, timeout);
		if (st != HAL_OK) {
			if (st == HAL_TIMEOUT)
				return CTIA_TIMEOUT;
			if (st == HAL_BUSY)
				return CTIA_BUSY;
			return CTIA_FAIL;
		}
    }

    if (bytes_to_read == 0)
        return CTIA_SUCCESS;

    while ((HAL_GetTick() - start_tick) < timeout) {
        uint32_t pos = dma_rx_pos();

        uint32_t received = (pos >= start_pos)
            ? (pos - start_pos)
            : (UART_RX_BUFFER_SIZE - start_pos + pos);

        if (received >= bytes_to_read) {
            for (uint32_t i = 0; i < bytes_to_read; i++) {
                uint32_t idx = (start_pos + i) % UART_RX_BUFFER_SIZE;
                buffer[i] = uart_rx_buffer[idx];
            }

            *size = bytes_to_read;
            return CTIA_SUCCESS;
        }
    }

    uint32_t pos = dma_rx_pos();
    uint32_t received = (pos >= start_pos)
        ? (pos - start_pos)
        : (UART_RX_BUFFER_SIZE - start_pos + pos);

    if (received > 0) {
        uint32_t count = (received > bytes_to_read) ? bytes_to_read : received;

        for (uint32_t i = 0; i < count; i++) {
            uint32_t idx = (start_pos + i) % UART_RX_BUFFER_SIZE;
            buffer[i] = uart_rx_buffer[idx];
        }

        *size = count;
        return CTIA_SUCCESS;   // partial data accepted
    }

    return CTIA_TIMEOUT;
}
