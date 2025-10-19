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

typedef enum _resp_cmd_s {
	RESP_OK	= 0x0101,
	RESP_ERROR,
	RESP_INVALID_CMD,
	RESP_INVALID_PARAM,
	RESP_TIMEOUT,
	RESP_BUSY,
	RESP_UNAVAILABLE,
	RESP_END = 0x01FF
} resp_cmd_t;

typedef enum _set_cmd_s {
	SET_MEAS_CH_H = 0x0201,
	SET_MEAS_CH_L,
	SET_STIM_CH,
	SET_EXT_PROBE_IN,
	SET_EXT_STIM_1,
	SET_EXT_STIM_2,
	SET_EXT_STIM_3,
	SET_EXT_STIM_4,
	SET_END = 0x02FF
} set_cmd_t;

typedef enum _clr_cmd_s {
	CLR_MEAS_CH_H = 0x0301,
	CLR_MEAS_CH_L,
	CLR_STIM_CH,
	CLR_EXT_PROBE_IN,
	CLR_EXT_STIM_1,
	CLR_EXT_STIM_2,
	CLR_EXT_STIM_3,
	CLR_EXT_STIM_4,
	CLR_END = 0x03FF
} clr_cmd_t;

typedef enum _get_cmd_s {
	GET_MEAS_CH_H = 0x0401,
	GET_MEAS_CH_L,
	GET_STIM_CH,
	GET_EXT_PROBE_IN,
	GET_EXT_STIM_1,
	GET_EXT_STIM_2,
	GET_EXT_STIM_3,
	GET_EXT_STIM_4,
	GET_END = 0x04FF
} get_cmd_t;

typedef enum _conf_cmd_s {
	CONF_SET_AVAILABLE_MEAS_CH = 0x0501,
	CONF_GET_AVAILABLE_MEAS_CH,
	CONF_END = 0x05FF
} conf_cmd_t;

typedef enum _exec_cmd_s {
	EXAMPLE_RUN_CMD	= 0x0601,
	EXEC_END = 0x06FF
} exec_cmd_t;

typedef enum _dbg_cmd_s {
	EXAMPLE_DBG_CMD	= 0x0701,
	DBG_END = 0x07FF
} dbg_cmd_t;

#endif /* COMMAND_REFERENCE_H_ */
