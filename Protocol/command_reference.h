/*
 * command_reference.h
 *
 *  Created on: Oct 18, 2025
 *      Author: tobias
 */

#ifndef COMMAND_REFERENCE_H_
#define COMMAND_REFERENCE_H_

typedef enum _cmd_status_s {
	CMD_SUCCESS,
	CMD_FAIL,
	CMD_TOO_FEW_BYTES,
	CMD_TOO_MANY_BYTES,
	CMD_CRC_MISSMATCH
} cmd_status_t;

typedef enum _set_cmd_s {
	SET_MEAS_CH
} set_cmd_t;

#endif /* COMMAND_REFERENCE_H_ */
