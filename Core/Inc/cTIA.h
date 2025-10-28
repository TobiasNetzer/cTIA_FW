/*
 * cTIA.h
 *
 *  Created on: Oct 19, 2025
 *      Author: tobias
 */

#ifndef CTIA_H_
#define CTIA_H_

#include <stdint.h>
#include "command_reference.h"
#include "TLC6C5816_driver.h"

/* These should be saved in EEPROM later for configuring custom units! */
#define STIM_CH_COUNT 16
#define MEAS_CH_COUNT 32
#define EXT_STIM_CH_COUNT 4

typedef struct _ctia_state_s {
	uint8_t active_stim_ch_bitfield[MAX_CH_COUNT / 8];
	uint8_t active_meas_h_ch_bitfield[MAX_CH_COUNT / 8];
	uint8_t active_meas_l_ch_bitfield[MAX_CH_COUNT / 8];
	uint8_t active_ext_stim_ch_bitfield[1 + (EXT_STIM_CH_COUNT / 8)];
	uint8_t ext_probe_in_state;
	uint8_t ext_trigger_state;
} ctia_state_t;

ctia_status_t cTIA_set_exclusive_meas_h_ch(uint8_t channel);

ctia_status_t cTIA_set_meas_h_ch(uint8_t channel);

ctia_status_t cTIA_set_meas_h_ch_bitfield(const uint8_t *payload, uint8_t size);

ctia_status_t cTIA_set_exclusive_meas_l_ch(uint8_t channel);

ctia_status_t cTIA_set_meas_l_ch(uint8_t channel);

ctia_status_t cTIA_set_meas_l_ch_bitfield(const uint8_t *payload, uint8_t size);

ctia_status_t cTIA_set_exclusive_stim_ch(uint8_t channel);

ctia_status_t cTIA_set_stim_ch(uint8_t channel);

ctia_status_t cTIA_set_stim_ch_bitfield(const uint8_t *payload, uint8_t size);

ctia_status_t cTIA_set_ext_probe_in(uint8_t state);

ctia_status_t cTIA_set_ext_trigger(uint8_t state);

ctia_status_t cTIA_set_ext_stim_ch(uint8_t channel);

ctia_status_t cTIA_set_ext_stim_ch_bitfield(const uint8_t *payload, uint8_t size);

ctia_status_t cTIA_clear_ext_stim_ch(uint8_t channel);

#endif /* CTIA_H_ */
