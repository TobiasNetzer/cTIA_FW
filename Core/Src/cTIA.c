/*
 * cTIA.c
 *
 *  Created on: Oct 19, 2025
 *      Author: tobias
 */

#include <cTIA.h>
#include "main.h"
#include <string.h>

static ctia_state_t ctia_state = {0};

ctia_status_t cTIA_set_exclusive_meas_h_ch(uint8_t channel) {

	if (channel > MEAS_CH_COUNT) return CTIA_UNAVAILABLE;

	memset(ctia_state.active_meas_h_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_h_ch_bitfield));

	uint8_t index = (channel - 1) / 8;

	ctia_state.active_meas_h_ch_bitfield[index] |= (1 << (channel - 1));

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_exclusive_output_channel(channel);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_meas_h_ch(uint8_t channel) {

	if (channel > MEAS_CH_COUNT) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_meas_h_ch_bitfield[index] |= (1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_h_ch_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_meas_h_ch_bitfield(const uint8_t *payload, uint8_t size) {

	if (size == 0) return CTIA_INVALID_PARAM;

	memset(ctia_state.active_meas_h_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_h_ch_bitfield));
	memcpy(ctia_state.active_meas_h_ch_bitfield, payload, size);

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_h_ch_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_exclusive_meas_l_ch(uint8_t channel) {

	if (channel > MEAS_CH_COUNT) return CTIA_UNAVAILABLE;

	memset(ctia_state.active_meas_l_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_l_ch_bitfield));

	uint8_t index = (channel - 1) / 8;

	ctia_state.active_meas_l_ch_bitfield[index] |= (1 << (channel - 1));

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_exclusive_output_channel(channel);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_meas_l_ch(uint8_t channel) {

	if (channel > MEAS_CH_COUNT) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_meas_l_ch_bitfield[index] |= (1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_l_ch_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_meas_l_ch_bitfield(const uint8_t *payload, uint8_t size) {

	if (size == 0) return CTIA_INVALID_PARAM;

	memset(ctia_state.active_meas_l_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_l_ch_bitfield));
	memcpy(ctia_state.active_meas_l_ch_bitfield, payload, size);

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_l_ch_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_exclusive_stim_ch(uint8_t channel) {

	if (channel > STIM_CH_COUNT) return CTIA_UNAVAILABLE;

	memset(ctia_state.active_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_stim_ch_bitfield));

	uint8_t index = (channel - 1) / 8;

	ctia_state.active_stim_ch_bitfield[index] |= (1 << (channel - 1));

	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_exclusive_output_channel(channel);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_stim_ch(uint8_t channel) {

	if (channel > STIM_CH_COUNT) return CTIA_UNAVAILABLE;

	uint8_t index = (channel - 1) / 8;
	uint8_t bit_pos = (channel - 1) % 8;

	ctia_state.active_stim_ch_bitfield[index] |= (1 << bit_pos);

	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_stim_ch_bitfield, sizeof(ctia_state.active_stim_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_stim_ch_bitfield(const uint8_t *payload, uint8_t size) {

	if (size == 0) return CTIA_INVALID_PARAM;

	memset(ctia_state.active_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_stim_ch_bitfield));
	memcpy(ctia_state.active_stim_ch_bitfield, payload, size);

	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_stim_ch_bitfield, sizeof(ctia_state.active_stim_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_RESET);

	return status;
}

ctia_status_t cTIA_set_ext_probe_in(uint8_t state) {

	if (state > 1) return CTIA_INVALID_PARAM;
	ctia_state.ext_probe_in_state = state;

	HAL_GPIO_WritePin(EXT_PROBE_ON_GPIO_Port, EXT_PROBE_ON_Pin, ctia_state.ext_probe_in_state);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_set_ext_trigger(uint8_t state) {

	if (state > 1) return CTIA_INVALID_PARAM;
	ctia_state.ext_trigger_state = state;

	HAL_GPIO_WritePin(EXT_TRIG_INT_GPIO_Port, EXT_TRIG_INT_Pin, ctia_state.ext_trigger_state);

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_set_ext_stim_ch(uint8_t channel) {

	if (channel > EXT_STIM_CH_COUNT || channel == 0) return CTIA_UNAVAILABLE;

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

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_set_ext_stim_ch_bitfield(const uint8_t *payload, uint8_t size) {

	if (size > (1 + (EXT_STIM_CH_COUNT / 8)) || size == 0) return CTIA_UNAVAILABLE;
	if (!payload) return CTIA_INVALID_PARAM;

	ctia_status_t status;

	memset(ctia_state.active_ext_stim_ch_bitfield, 0x00, sizeof(ctia_state.active_ext_stim_ch_bitfield));

	for (uint8_t byte = 0; byte < size; byte++) {
		for (uint8_t bit = 0; bit < 8; bit++) {
			if (payload[byte] & (1 << bit)) {
				status = cTIA_set_ext_stim_ch(byte * 8 + bit + 1);
				if (status != CTIA_SUCCESS) return status;
			} else {
				status = cTIA_clear_ext_stim_ch(byte * 8 + bit + 1);
				if (status == CTIA_UNAVAILABLE) continue; // Since data is transmitted as uint8_t, the return parameter CTIA_UNAVAILABLE is inevitable because we have no way of telling how many bits are "valid".
			}
		}
	}

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_clear_ext_stim_ch(uint8_t channel) {

	if (channel > EXT_STIM_CH_COUNT || channel == 0) return CTIA_UNAVAILABLE;

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

	return CTIA_SUCCESS;
}
