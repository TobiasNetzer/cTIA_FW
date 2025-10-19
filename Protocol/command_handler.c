/*
 * command_handler.c
 *
 *  Created on: Oct 19, 2025
 *      Author: tobias
 */

#include "command_handler.h"
#include "command_reference.h"
#include "main.h" // needed for GPIO #defines
#include "cTIA.h"

void handle_command(const cmd_frame_t *frame) {

	if (frame == NULL) return;

	switch (frame->command) {

		/** SET CMD **/
		case SET_STIM_CH: {
			set_stim_ch();
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** SET CMD **/
		case SET_EXT_STIM_1: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_1_ON_Pin, GPIO_PIN_SET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** SET CMD **/
		case SET_EXT_STIM_2: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_2_ON_Pin, GPIO_PIN_SET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** SET CMD **/
		case SET_EXT_STIM_3: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_3_ON_Pin, GPIO_PIN_SET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** SET CMD **/
		case SET_EXT_STIM_4: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_4_ON_Pin, GPIO_PIN_SET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** CLR CMD **/
		case CLR_EXT_STIM_1: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_1_ON_Pin, GPIO_PIN_RESET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** CLR CMD **/
		case CLR_EXT_STIM_2: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_2_ON_Pin, GPIO_PIN_RESET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** CLR CMD **/
		case CLR_EXT_STIM_3: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_3_ON_Pin, GPIO_PIN_RESET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		/** CLR CMD **/
		case CLR_EXT_STIM_4: {
			HAL_GPIO_WritePin(GPIOD, EXT_STIM_4_ON_Pin, GPIO_PIN_RESET);
			cmd_transmit_no_payload(RESP_OK);
			break;
		}

		default: {
			cmd_transmit_no_payload(RESP_INVALID_CMD);
		}

	}

}
