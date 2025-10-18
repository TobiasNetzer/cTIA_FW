/*
 * uart_protocol_layer.c
 *
 *  Created on: Oct 18, 2025
 *      Author: tobias
 */

#include "cmd_protocol_layer.h"
#include "command_reference.h"
#include "stm32g0xx_hal.h"
#include "usbd_cdc_if.h"

extern CRC_HandleTypeDef hcrc;

static uint8_t parse_cmd(uint8_t *input_buffer, uint8_t size, cmd_frame_t *cmd_frame) {
	if (!input_buffer) return CMD_FAIL;
	if (size < MIN_FRAME_SIZE) return CMD_TOO_FEW_BYTES;
	if (size > MAX_FRAME_SIZE) return CMD_TOO_MANY_BYTES;

	if (input_buffer[0] != (uint8_t) (CMD_FRAME_HEADER & 0xFF) || input_buffer[1] != (uint8_t) ((CMD_FRAME_HEADER >> 8) & 0xFF)) return CMD_FAIL;

	while (HAL_CRC_GetState(&hcrc) == HAL_CRC_STATE_BUSY); // wait if crc is busy

	uint8_t received_crc8 = input_buffer[size - 1];
	uint8_t crc8 = (uint8_t) HAL_CRC_Calculate(&hcrc, (uint32_t *) &input_buffer[2], (uint32_t) size - 3);

	if (received_crc8 != crc8) return CMD_CRC_MISSMATCH;

	cmd_frame->header = ((uint16_t)input_buffer[1] << 8) | input_buffer[0];
	cmd_frame->command = ((uint16_t)input_buffer[3] << 8) | input_buffer[2];
	cmd_frame->control_byte = input_buffer[4];
	cmd_frame->payload_size = input_buffer[5];

	for (uint8_t i = 0; i < cmd_frame->payload_size; i++) {
		cmd_frame->payload[i] = input_buffer[i + 6];
	}

	cmd_frame->crc = received_crc8;

	return CMD_SUCCESS;

}

uint8_t cmd_transmit(uint16_t cmd, uint8_t ctrl_byte, uint8_t payload_size, uint8_t *payload) {

	uint8_t transmit_buffer[MAX_FRAME_SIZE];
	uint8_t frame_size = 0;
	uint8_t retries = 0;

	transmit_buffer[frame_size++] = (uint8_t) (CMD_FRAME_HEADER & 0xFF); //low byte
	transmit_buffer[frame_size++] = (uint8_t) ((CMD_FRAME_HEADER >> 8) & 0xFF); //high byte
	transmit_buffer[frame_size++] = (uint8_t) (cmd & 0xFF); //low byte
	transmit_buffer[frame_size++] = (uint8_t) ((cmd >> 8) & 0xFF); //high byte
	transmit_buffer[frame_size++] = ctrl_byte;
	transmit_buffer[frame_size++] = payload_size;

	for (uint8_t i = 0; i < payload_size; i++) {
		transmit_buffer[frame_size++] = payload[i];
	}

	while (HAL_CRC_GetState(&hcrc) == HAL_CRC_STATE_BUSY); // wait if crc is busy

	uint8_t crc8 = (uint8_t) HAL_CRC_Calculate(&hcrc, (uint32_t *) &transmit_buffer[2], (uint32_t) frame_size - 2);
	transmit_buffer[frame_size++] = crc8;

	do {
		uint8_t status = CDC_Transmit_FS(transmit_buffer, frame_size);
		if (status == USBD_OK) {
			return CMD_SUCCESS;
		} else if (status == USBD_BUSY) {
			retries++;
			HAL_Delay(TRANSMIT_RETRY_DELAY_MS);
		} else {
			return CMD_FAIL;
		}
	} while (retries < MAX_TRANSMIT_RETRIES);

	return CMD_FAIL;
}

void cmd_receive_callback(uint8_t *buf, uint32_t *len) {
	cmd_frame_t cmd_frame;
	uint8_t status = parse_cmd(buf, *len, &cmd_frame);

	if (status == CMD_SUCCESS) cmd_transmit(0x0001, 0x00, 0x01, (uint8_t *) 0x01);
	else cmd_transmit(0x0001, 0x00, 0x01, (uint8_t *) 0x02);
}
