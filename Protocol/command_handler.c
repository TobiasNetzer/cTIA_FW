/*
 * command_handler.c
 *
 *  Created on: Oct 19, 2025
 *      Author: tobias
 */

#include "command_handler.h"
#include "cTIA.h"
#include "command_reference.h"
#include <stdbool.h>

void handle_command(cmd_frame_t *frame) {

	if (frame == NULL) return;

	ctia_status_t status = CTIA_SUCCESS;
	bool send_response = !(frame->control_byte & CONTROL_BYTE_DONT_SEND_RESPONSE);

	switch (frame->command) {

		/** SET CMD **/
		case SET_EXCLUSIVE_MEAS_H_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_exclusive_meas_h_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_MEAS_H_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_meas_h_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_MEAS_H_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_meas_h_ch_bitfield(frame->payload, frame->payload_size);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_EXCLUSIVE_MEAS_L_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_exclusive_meas_l_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_MEAS_L_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_meas_l_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_MEAS_L_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_meas_l_ch_bitfield(frame->payload, frame->payload_size);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_EXCLUSIVE_STIM_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_exclusive_stim_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_STIM_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_stim_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_STIM_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_stim_ch_bitfield(frame->payload, frame->payload_size);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_EXT_STIM_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_ext_stim_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_EXT_STIM_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_ext_stim_ch_bitfield(frame->payload, frame->payload_size);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_EXT_PROBE_IN: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_ext_probe_in(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** SET CMD **/
		case SET_EXT_TRIGGER: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_ext_trigger(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_MEAS_H_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_clear_meas_h_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_MEAS_H: {
			status = cTIA_clear_meas_h();
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_MEAS_L_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_clear_meas_l_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_MEAS_L: {
			status = cTIA_clear_meas_l();
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_STIM_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_clear_stim_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_STIM: {
			status = cTIA_clear_stim();
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_EXT_STIM_CH: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_clear_ext_stim_ch(frame->payload[0]);
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		/** CLR CMD **/
		case CLR_EXT_STIM: {
			status = cTIA_clear_ext_stim();
			frame->command = RESP_OK;
			frame->control_byte = 0;
			frame->payload_size = 0;
			break;
		}

		default: {
			status = CTIA_INVALID_CMD;
		}

	}

	if (status != CTIA_SUCCESS) goto error;

	if (send_response) cmd_transmit(frame);
	return;

error:
 	frame->command = RESP_ERROR;
 	frame->control_byte = 0;
	frame->payload_size = 1;
	frame->payload[0] = status;
	if (send_response) cmd_transmit(frame);
	return;
}
