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

extern UART_HandleTypeDef huart1;
extern device_config_t cTIA_config;

static ctia_state_t ctia_state = {0};

ctia_status_t cTIA_set_exclusive_meas_h_ch(uint8_t channel) {

	if (channel > cTIA_config.meas_ch_count || channel == 0) return CTIA_UNAVAILABLE;

	uint8_t old_bitfield[sizeof(ctia_state.active_meas_h_ch_bitfield)];
	memcpy(old_bitfield, ctia_state.active_meas_h_ch_bitfield, sizeof(old_bitfield));

	memset(ctia_state.active_meas_h_ch_bitfield, 0x00, sizeof(ctia_state.active_meas_h_ch_bitfield));

	uint8_t index = (channel - 1) / 8;
	ctia_state.active_meas_h_ch_bitfield[index] |= (1 << (channel - 1));

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
	ctia_state.active_meas_l_ch_bitfield[index] |= (1 << (channel - 1));

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
	ctia_state.active_stim_ch_bitfield[index] |= (1 << (channel - 1));

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


ctia_status_t cTIA_set_ext_probe_in(uint8_t state) {

	if (state > 1) return CTIA_INVALID_PARAMETER;
	ctia_state.ext_probe_in_state = state;

	HAL_GPIO_WritePin(EXT_PROBE_ON_GPIO_Port, EXT_PROBE_ON_Pin, ctia_state.ext_probe_in_state);

	HAL_Delay(5); // Make sure Relay has enough time to switch

	return CTIA_SUCCESS;
}

ctia_status_t cTIA_set_ext_trigger(uint8_t state) {

	if (state > 1) return CTIA_INVALID_PARAMETER;
	ctia_state.ext_trigger_state = state;

	HAL_GPIO_WritePin(EXT_TRIG_INT_GPIO_Port, EXT_TRIG_INT_Pin, ctia_state.ext_trigger_state);

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

		HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_RESET);
		HAL_Delay(5);
		ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_h_ch_bitfield, sizeof(ctia_state.active_meas_h_ch_bitfield));
		HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_H_GPIO_Port, SHIFT_REG_RCK_MEAS_H_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_H_GPIO_Port, SHIFT_REG_G_MEAS_H_Pin, GPIO_PIN_RESET);

		return status;
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

	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_meas_l_ch_bitfield, sizeof(ctia_state.active_meas_l_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_MEAS_L_GPIO_Port, SHIFT_REG_RCK_MEAS_L_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_MEAS_L_GPIO_Port, SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_RESET);

	return status;
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

	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	ctia_status_t status = tlc6c5816_set_output_channel_bitfield(ctia_state.active_stim_ch_bitfield, sizeof(ctia_state.active_stim_ch_bitfield));
	HAL_GPIO_WritePin(SHIFT_REG_RCK_STIM_GPIO_Port, SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SHIFT_REG_G_STIM_GPIO_Port, SHIFT_REG_G_STIM_Pin, GPIO_PIN_RESET);

	return status;
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
	cTIA_set_ext_probe_in(0);

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

ctia_status_t cTIA_get_ext_trigger_state(uint8_t *state) {

	if (state == NULL) return CTIA_INVALID_PARAMETER;

	*state = ctia_state.ext_trigger_state;

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

ctia_status_t cTIA_uart_transmit(uint8_t *buffer, uint8_t size) {

	if (buffer == NULL || size == 0) return CTIA_INVALID_PARAMETER;

	HAL_UART_Transmit(&huart1, buffer, size, 1000);

	return CTIA_SUCCESS;
}
