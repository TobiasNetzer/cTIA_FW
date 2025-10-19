/*
 * uart_protocol_layer.h
 *
 *  Created on: Oct 18, 2025
 *      Author: tobias
 */

#ifndef COMMAND_PROTOCOL_LAYER_H_
#define COMMAND_PROTOCOL_LAYER_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_TRANSMIT_RETRIES 10
#define TRANSMIT_RETRY_DELAY_MS 10

#define CMD_FRAME_HEADER 0xAA55

#define MAX_FRAME_SIZE 128
#define MIN_FRAME_SIZE 7
#define MAX_PAYLOAD_SIZE 120

typedef struct _cmd_frame_s {
	uint16_t header;
	uint16_t command;
	uint8_t control_byte;
	uint8_t payload_size;
	uint8_t payload[MAX_PAYLOAD_SIZE];
	uint8_t crc;
} cmd_frame_t;

uint8_t cmd_transmit(uint16_t cmd, uint8_t ctrl_byte, uint8_t payload_size, uint8_t *payload);

uint8_t cmd_transmit_no_payload(uint16_t cmd);

void get_frame(cmd_frame_t** frame);

bool get_frame_ready(void);

void clear_frame_ready(void);

#endif /* COMMAND_PROTOCOL_LAYER_H_ */
