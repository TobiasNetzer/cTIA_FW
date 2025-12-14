/*
 * uart_protocol_layer.c
 *
 *  Created on: Oct 18, 2025
 *      Author: tobias
 */

#include "command_protocol_layer.h"
#include "stm32g0xx_hal.h"
#include "usbd_cdc_if.h"
#include <stdbool.h>

static uint8_t transmit_buffer[MAX_FRAME_SIZE];

static cmd_frame_t cmd_frame;
static volatile bool frame_ready = false;

extern CRC_HandleTypeDef hcrc;
extern void Error_Handler(void);

static ctia_status_t parse_cmd(uint8_t *input_buffer, uint8_t size) {

	if (!input_buffer) return CTIA_FAIL;
	if (size < MIN_FRAME_SIZE) return CTIA_TOO_FEW_BYTES;
	if (size > MAX_FRAME_SIZE) return CTIA_TOO_MANY_BYTES;

	if (input_buffer[0] != (uint8_t) (CMD_FRAME_HEADER & 0xFF) || input_buffer[1] != (uint8_t) ((CMD_FRAME_HEADER >> 8) & 0xFF)) return CTIA_FAIL;

	if (!(input_buffer[4] & CONTROL_BYTE_IGNORE_CRC_BYTE)) {

		while (HAL_CRC_GetState(&hcrc) == HAL_CRC_STATE_BUSY); // wait if crc is busy

		uint8_t received_crc8 = input_buffer[size - 1];
		uint8_t crc8 = (uint8_t) HAL_CRC_Calculate(&hcrc, (uint32_t *) &input_buffer[2], (uint32_t) size - 3);

		if (received_crc8 != crc8) return CTIA_CRC_MISMATCH;

	}

	cmd_frame.header = ((uint16_t)input_buffer[1] << 8) | input_buffer[0];
	cmd_frame.command = ((uint16_t)input_buffer[3] << 8) | input_buffer[2];
	cmd_frame.control_byte = input_buffer[4];
	cmd_frame.payload_size = input_buffer[5];

	for (uint8_t i = 0; i < cmd_frame.payload_size; i++) {
		cmd_frame.payload[i] = input_buffer[i + 6];
	}

	cmd_frame.crc = input_buffer[size - 1];

	return CTIA_SUCCESS;

}

ctia_status_t cmd_transmit(cmd_frame_t *frame) {

	uint8_t frame_size = 0;
	uint8_t retries = 0;

	transmit_buffer[frame_size++] = (uint8_t) (CMD_FRAME_HEADER & 0xFF); //low byte
	transmit_buffer[frame_size++] = (uint8_t) ((CMD_FRAME_HEADER >> 8) & 0xFF); //high byte
	transmit_buffer[frame_size++] = (uint8_t) (frame->command & 0xFF); //low byte
	transmit_buffer[frame_size++] = (uint8_t) ((frame->command >> 8) & 0xFF); //high byte
	transmit_buffer[frame_size++] = frame->control_byte;
	transmit_buffer[frame_size++] = frame->payload_size;

	for (uint8_t i = 0; i < frame->payload_size; i++) {
		transmit_buffer[frame_size++] = frame->payload[i];
	}

	while (HAL_CRC_GetState(&hcrc) == HAL_CRC_STATE_BUSY); // wait if crc is busy

	uint8_t crc8 = (uint8_t) HAL_CRC_Calculate(&hcrc, (uint32_t *) &transmit_buffer[2], (uint32_t) frame_size - 2);
	transmit_buffer[frame_size++] = crc8;

	do {
		uint8_t status = CDC_Transmit_FS(transmit_buffer, frame_size);
		if (status == USBD_OK) {
			return CTIA_SUCCESS;
		} else if (status == USBD_BUSY) {
			retries++;
			HAL_Delay(TRANSMIT_RETRY_DELAY_MS);
		} else {
			return CTIA_FAIL;
		}
	} while (retries < MAX_TRANSMIT_RETRIES);

	return CTIA_FAIL;

}

void get_frame(cmd_frame_t **frame) {

    if (frame_ready) {
        *frame = &cmd_frame;
    } else {
        *frame = NULL;
    }

}

bool get_frame_ready(void) {

	return frame_ready;

}

void clear_frame_ready(void) {

	frame_ready = false;

}

void cmd_receive_callback(uint8_t *buf, uint32_t *len) {

	if (frame_ready) {
		/* This "dummy" frame contains a busy response.
		 * Since the cmd_frame struct is not allowed to be accessed or modified in this state,
		 * where the command_handler hasn't finished yet, we would need to create a temporary struct
		 * that can be passed to the cmd_transmit function. However this would allocate 128 Bytes on the Stack.
		 * Therefore a minimal frame is created and cast to cmd_frame_t *.
		 */
		uint8_t busy_frame[7] = {0x00, 0x00, 0x02, 0x01, 0x00, 0x01, 0x08};
		if (cmd_transmit((cmd_frame_t *) &busy_frame) != CTIA_SUCCESS) Error_Handler(); // cmd_handler not finished - If transmit fails handle error
		return;
	}

	ctia_status_t status = parse_cmd(buf, *len);

	if (status != CTIA_SUCCESS) {
		cmd_frame.command = RESP_ERROR;
		cmd_frame.payload_size = 1;
		cmd_frame.payload[0] = status;
		if (cmd_transmit(&cmd_frame) != CTIA_SUCCESS) Error_Handler();
		return;
	}

	frame_ready = true;

}
