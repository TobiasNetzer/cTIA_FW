/*
 * command_handler.c
 *
 *  Created on: Oct 19, 2025
 *      Author: tobias
 */

#include "command_handler.h"
#include "cTIA.h"
#include "command_reference.h"
#include "bootloader.h"
#include <stdbool.h>

static inline void cmd_frame_response_ok(cmd_frame_t *frame) {
    frame->command = RESP_OK;
    frame->payload_size = 0;
}

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
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_MEAS_H_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_meas_h_ch_bitfield(frame->payload, frame->payload_size);
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_MEAS_L_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_meas_l_ch_bitfield(frame->payload, frame->payload_size);
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_STIM_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_stim_ch_bitfield(frame->payload, frame->payload_size);
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** SET CMD **/
		case SET_BITFIELD_EXT_STIM_CH: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_set_ext_stim_ch_bitfield(frame->payload, frame->payload_size);
			cmd_frame_response_ok(frame);
			break;
		}

		/** SET CMD **/
		case SET_EXT_PROBE_IN_STATE: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_ext_probe_in_state(frame->payload[0]);
			cmd_frame_response_ok(frame);
			break;
		}

		/** SET CMD **/
		case SET_ANALOG_BUS_DETECT_STATE: {
			if (frame->payload_size < 1) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			if (frame->payload_size > 1) {
				status = CTIA_TOO_MANY_BYTES;
				goto error;
			}
			status = cTIA_set_analog_bus_detect_state(frame->payload[0]);
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** CLR CMD **/
		case CLR_MEAS_H: {
			status = cTIA_clear_meas_h();
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** CLR CMD **/
		case CLR_MEAS_L: {
			status = cTIA_clear_meas_l();
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** CLR CMD **/
		case CLR_STIM: {
			status = cTIA_clear_stim();
			cmd_frame_response_ok(frame);
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
			cmd_frame_response_ok(frame);
			break;
		}

		/** CLR CMD **/
		case CLR_EXT_STIM: {
			status = cTIA_clear_ext_stim();
			cmd_frame_response_ok(frame);
			break;
		}

		/** CLR CMD **/
		case CLR_ALL_RELAYS: {
			status = cTIA_clear_all_relays();
			cmd_frame_response_ok(frame);
			break;
		}

		/** GET CMD **/
		case GET_DEVICE_ID: {
			status = cTIA_get_device_id(frame->payload, &frame->payload_size);
			frame->command = RESP_DEVICE_ID;
			break;
		}

		/** GET CMD **/
		case GET_DEVICE_NAME: {
			status = cTIA_get_device_name(frame->payload, &frame->payload_size);
			frame->command = RESP_DEVICE_NAME;
			break;
		}

		/** GET CMD **/
		case GET_SERIAL_NUMBER: {
			status = cTIA_get_serial_number(frame->payload, &frame->payload_size);
			frame->command = RESP_SERIAL_NUMBER;
			break;
		}

		/** GET CMD **/
		case GET_FW_BUILD_DATE: {
			status = cTIA_get_fw_build_date(frame->payload, &frame->payload_size);
			frame->command = RESP_FW_BUILD_DATE;
			break;
		}

		/** GET CMD **/
		case GET_FW_BUILD_TIME: {
			status = cTIA_get_fw_build_time(frame->payload, &frame->payload_size);
			frame->command = RESP_FW_BUILD_TIME;
			break;
		}

		/** GET CMD **/
		case GET_FW_VERSION: {
			status = cTIA_get_fw_version(frame->payload, &frame->payload_size);
			frame->command = RESP_FW_VERSION;
			break;
		}

		/** GET CMD **/
		case GET_AVAILABLE_MEAS_CH: {
			status = cTIA_get_available_meas_channels(frame->payload, &frame->payload_size);
			frame->command = RESP_AVAILABLE_MEAS_CH;
			break;
		}

		/** GET CMD **/
		case GET_AVAILABLE_STIM_CH: {
			status = cTIA_get_available_stim_channels(frame->payload, &frame->payload_size);
			frame->command = RESP_AVAILABLE_STIM_CH;
			break;
		}

		/** GET CMD **/
		case GET_AVAILABLE_EXT_STIM_CH: {
			status = cTIA_get_available_ext_stim_channels(frame->payload, &frame->payload_size);
			frame->command = RESP_AVAILABLE_EXT_STIM_CH;
			break;
		}

		/** GET CMD **/
		case GET_BITFIELD_MEAS_H: {
			status = cTIA_get_meas_h_bitfield(frame->payload, &frame->payload_size);
			frame->command = RESP_BITFIELD_MEAS_H;
			break;
		}

		/** GET CMD **/
		case GET_BITFIELD_MEAS_L: {
			status = cTIA_get_meas_l_bitfield(frame->payload, &frame->payload_size);
			frame->command = RESP_BITFIELD_MEAS_L;
			break;
		}

		/** GET CMD **/
		case GET_BITFIELD_STIM: {
			status = cTIA_get_stim_bitfield(frame->payload, &frame->payload_size);
			frame->command = RESP_BITFIELD_STIM;
			break;
		}

		/** GET CMD **/
		case GET_BITFIELD_EXT_STIM: {
			status = cTIA_get_ext_stim_bitfield(frame->payload, &frame->payload_size);
			frame->command = RESP_BITFIELD_EXT_STIM;
			break;
		}

		/** GET CMD **/
		case GET_EXT_PROBE_IN_STATE: {
			status = cTIA_get_ext_probe_in_state(&frame->payload[0]);
			frame->command = RESP_EXT_PROBE_IN_STATE;
			frame->payload_size = 1;
			break;
		}

		/** GET CMD **/
		case GET_ANALOG_BUS_DETECT_STATE: {
			status = cTIA_get_analog_bus_detect_state(&frame->payload[0]);
			frame->command = RESP_ANALOG_BUS_DETECT_STATE;
			frame->payload_size = 1;
			break;
		}

		/** CONF CMD **/
		case CONF_SERIAL_NUMBER: {
			status = cTIA_conf_serial_number(frame->payload, &frame->payload_size);
			cmd_frame_response_ok(frame);
			break;
		}

		/** CONF CMD **/
		case CONF_AVAILABLE_MEAS_CH: {
			status = cTIA_conf_available_meas_ch(frame->payload[0]);
			cmd_frame_response_ok(frame);
			break;
		}

		/** CONF CMD **/
		case CONF_AVAILABLE_STIM_CH: {
			status = cTIA_conf_available_stim_ch(frame->payload[0]);
			cmd_frame_response_ok(frame);
			break;
		}

		/** CONF CMD **/
		case CONF_AVAILABLE_EXT_STIM_CH: {
			status = cTIA_conf_available_ext_stim_ch(frame->payload[0]);
			cmd_frame_response_ok(frame);
			break;
		}

		case EXECUTE_SELFTEST: {
			status = cTIA_execute_selftest(frame->payload, &frame->payload_size);
			frame->command = RESP_EXECUTE_SELFTEST;
			break;
		}

		/** UART CMD **/
		case UART_TRANSMIT: {
			if (frame->payload_size == 0) {
				status = CTIA_TOO_FEW_BYTES;
				goto error;
			}
			status = cTIA_uart_transmit(frame->payload, frame->payload_size);
			cmd_frame_response_ok(frame);
			break;
		}

		/** DBG CMD **/
		case DBG_ENTER_BOOTLOADER: {
			cTIA_clear_all_relays();
			JumpToBootloader();

			// should never run
			cmd_frame_response_ok(frame);
			break;
		}

		default: {
			status = CTIA_INVALID_CMD;
		}

	}

	if (status != CTIA_SUCCESS) goto error;

	frame->control_byte = 0;
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
