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

#define EXT_STIM_CH_MAX_COUNT 4

typedef struct _ctia_state_s {
	uint8_t active_stim_ch_bitfield[MAX_CH_COUNT / 8];
	uint8_t active_meas_h_ch_bitfield[MAX_CH_COUNT / 8];
	uint8_t active_meas_l_ch_bitfield[MAX_CH_COUNT / 8];
	uint8_t active_ext_stim_ch_bitfield[1 + (EXT_STIM_CH_MAX_COUNT / 8)];
	uint8_t ext_probe_in_state;
	uint8_t analog_bus_detect_state;
} ctia_state_t;


/**
 * @brief  Sets a exclusive measurement matrix high-channel.
 *
 * This function sets only the specified channel, resetting all other channels in the process and updates the internal bitfield.
 *
 * @param[in]  channel    		Measurement matrix high-channel to set.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_set_exclusive_meas_h_ch(uint8_t channel);

/**
 * @brief  Sets a measurement matrix high-channel.
 *
 * This function sets the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		Measurement matrix high-channel to set.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_set_meas_h_ch(uint8_t channel);

/**
 * @brief  Sets the measurement matrix high-channel bitfield from the provided payload.
 *
 * This function parses the input payload and updates the internal bitfield.
 *
 * @param[in]  payload 				Pointer to the buffer containing the bitfield data.
 * @param[in]  size    				Number of bytes in the payload buffer.
 *
 * @retval CTIA_SUCCESS            	Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   Payload pointer is NULL or size is invalid.
 * @retval CTIA_FAIL       			Operation failed.
 */
ctia_status_t cTIA_set_meas_h_ch_bitfield(const uint8_t *payload, uint8_t size);

/**
 * @brief  Sets a exclusive measurement matrix low-channel.
 *
 * This function sets only the specified channel, resetting all other channels in the process and updates the internal bitfield.
 *
 * @param[in]  channel    		Measurement matrix low-channel to set.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_set_exclusive_meas_l_ch(uint8_t channel);

/**
 * @brief  Sets a measurement matrix low-channel.
 *
 * This function sets the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		Measurement matrix low-channel to set.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_set_meas_l_ch(uint8_t channel);

/**
 * @brief  Sets the measurement matrix low-channel bitfield from the provided payload.
 *
 * This function parses the input payload and updates the internal bitfield.
 *
 * @param[in]  payload 				Pointer to the buffer containing the bitfield data.
 * @param[in]  size    				Number of bytes in the payload buffer.
 *
 * @retval CTIA_SUCCESS            	Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   Payload pointer is NULL or size is invalid.
 * @retval CTIA_FAIL       			Operation failed.
 */
ctia_status_t cTIA_set_meas_l_ch_bitfield(const uint8_t *payload, uint8_t size);

/**
 * @brief  Sets a exclusive stimulation channel.
 *
 * This function sets only the specified channel, resetting all other channels in the process and updates the internal bitfield.
 *
 * @param[in]  channel    		Stimulation channel to set.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_set_exclusive_stim_ch(uint8_t channel);

/**
 * @brief  Sets a stimulation channel.
 *
 * This function sets the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		Stimulation channel to set.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_set_stim_ch(uint8_t channel);

/**
 * @brief  Sets the stimulation channel bitfield from the provided payload.
 *
 * This function parses the input payload and updates the internal bitfield.
 *
 * @param[in]  payload 				Pointer to the buffer containing the bitfield data.
 * @param[in]  size    				Number of bytes in the payload buffer.
 *
 * @retval CTIA_SUCCESS            	Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   Payload pointer is NULL or size is invalid.
 * @retval CTIA_FAIL       			Operation failed.
 */
ctia_status_t cTIA_set_stim_ch_bitfield(const uint8_t *payload, uint8_t size);

/**
 * @brief  Enables/Disables the external probe input.
 *
 * This function controls whether the external probe is connected to MEAS CH 1.
 *
 * @param[in]  state    			0 = external probe is disconnected, 1 = external probe is connected to MEAS CH 1.
 *
 * @retval CTIA_SUCCESS         	Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   state value is invalid.
 */
ctia_status_t cTIA_set_ext_probe_in_state(uint8_t state);

/**
 * @brief  Enables/Disables the analog bus detect relay that is used for testing the relay matrix for shorts or defective relays.
 *
 * When the analog bus detect is enabled, the DMM H-L pins are disconnected from the DMM connector and rerouted to the internal detection circuitry.
 *
 * @param[in]  state    			0 = analog bus detect disabled, 1 = analog bus detect enabled.
 *
 * @retval CTIA_SUCCESS         	Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   state value is invalid.
 */
ctia_status_t cTIA_set_analog_bus_detect_state(uint8_t state);

/**
 * @brief  Sets a external stimulation source channel.
 *
 * This function sets the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		External stimulation channel to set.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 */
ctia_status_t cTIA_set_ext_stim_ch(uint8_t channel);

/**
 * @brief  Sets the external stimulation source channel bitfield from the provided payload.
 *
 * This function parses the input payload and updates the internal bitfield.
 *
 * @param[in]  payload 				Pointer to the buffer containing the bitfield data.
 * @param[in]  size    				Number of bytes in the payload buffer.
 *
 * @retval CTIA_SUCCESS            	Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   Payload pointer is NULL or size is invalid.
 * @retval CTIA_UNAVAILABLE       	The channel is not available.
 */
ctia_status_t cTIA_set_ext_stim_ch_bitfield(const uint8_t *payload, uint8_t size);

/**
 * @brief  Clears a measurement matrix high-channel.
 *
 * This function clears the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		Measurement matrix high-channel to clear.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_clear_meas_h_ch(uint8_t channel);

/**
 * @brief  Clears all measurement matrix high-channels.
 *
 * This function clears all measurement matrix high-channels and resets the internal bitfield.
 *
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_clear_meas_h(void);

/**
 * @brief  Clears a measurement matrix low-channel.
 *
 * This function clears the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		Measurement matrix low-channel to clear.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_clear_meas_l_ch(uint8_t channel);

/**
 * @brief  Clears all measurement matrix low-channels.
 *
 * This function clears all measurement matrix low-channels and resets the internal bitfield.
 *
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_clear_meas_l(void);

/**
 * @brief  Clears a stimulation channel.
 *
 * This function clears the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		Stimulation channel to clear.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_clear_stim_ch(uint8_t channel);

/**
 * @brief  Clears all stimulation channels.
 *
 * This function clears all stimulation channels and resets the internal bitfield.
 *
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_FAIL       		Operation failed.
 */
ctia_status_t cTIA_clear_stim(void);

/**
 * @brief  Clears a external stimulation source channel.
 *
 * This function clears the specified channel and updates the internal bitfield.
 *
 * @param[in]  channel    		External stimulation source channel to clear.
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 * @retval CTIA_UNAVAILABLE    	The channel is not available.
 */
ctia_status_t cTIA_clear_ext_stim_ch(uint8_t channel);

/**
 * @brief  Clears all external stimulation source channels.
 *
 * This function clears all external stimulation source channels and resets the internal bitfield.
 *
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 */
ctia_status_t cTIA_clear_ext_stim(void);

/**
 * @brief  Clears all relay states.
 *
 * This function clears all channels and resets the internal bitfields.
 *
 *
 * @retval CTIA_SUCCESS         Operation completed successfully.
 */
ctia_status_t cTIA_clear_all_relays(void);

/**
 * @brief  Retrieves the device ID.
 *
 * This function reads the unique device identifier from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the device ID will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_device_id(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the device name.
 *
 * This function reads the device name from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the device name will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_device_name(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the serial number.
 *
 * This function reads the serial number from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the serial number will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_serial_number(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the firmware build date.
 *
 * This function reads the firmware build date from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the firmware build date will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_fw_build_date(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the firmware build time.
 *
 * This function reads the firmware build time from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the firmware build time will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_fw_build_time(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the firmware version.
 *
 * This function reads the firmware version from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the firmware version will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_fw_version(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the internal bitfield for the measurement matrix high-channel.
 *
 * This function reads the internal bitfield for the measurement matrix high-channel from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the bitfield will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_meas_h_bitfield(uint8_t *bitfield, uint8_t *size);

/**
 * @brief  Retrieves the internal bitfield for the measurement matrix low-channel.
 *
 * This function reads the internal bitfield for the measurement matrix low-channel from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the bitfield will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_meas_l_bitfield(uint8_t *bitfield, uint8_t *size);

/**
 * @brief  Retrieves the internal bitfield for the stimulation channels.
 *
 * This function reads the internal bitfield for the stimulation channels from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the bitfield will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_stim_bitfield(uint8_t *bitfield, uint8_t *size);

/**
 * @brief  Retrieves the internal bitfield for the external stimulation source channels.
 *
 * This function reads the internal bitfield for the external stimulation source channels from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the bitfield will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_ext_stim_bitfield(uint8_t *bitfield, uint8_t *size);

/**
 * @brief  Retrieves the state of the external probe input.
 *
 * This function reads the state of the external probe input from the CTIA hardware
 * and stores it in the provided state buffer.
 *
 * @param[out] state 				Pointer to a buffer where the state will be stored.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_ext_probe_in_state(uint8_t *state);

/**
 * @brief  Retrieves the state of the analog bus detection relay.
 *
 * This function reads the state of the analog bus detection relay from the CTIA hardware
 * and stores it in the provided state buffer.
 *
 * @param[out] state 				Pointer to a buffer where the state will be stored.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_analog_bus_detect_state(uint8_t *state);

/**
 * @brief  Retrieves the available MEAS channels.
 *
 * This function reads the available number of MEAS channels from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the channel count will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_available_meas_channels(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the available STIM channels.
 *
 * This function reads the available number of STIM channels from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the channel count will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_available_stim_channels(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Retrieves the available EXT STIM channels.
 *
 * This function reads the available number of EXT STIM channels from the CTIA hardware
 * and stores it in the provided buffer. The actual number of bytes written
 * is returned through the @p size parameter.
 *
 * @param[out] buffer Pointer to a buffer where the channel count will be stored.
 * @param[out] size   Pointer to a variable that receives the number of bytes
 *                    written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL.
 */
ctia_status_t cTIA_get_available_ext_stim_channels(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Configures the device's serial number.
 *
 * This function reads a 32-bit serial number from the provided payload,
 * updates the internal device configuration, and persists the change to
 * non-volatile storage.
 *
 * The input buffer must contain exactly 4 bytes representing the new
 * serial number in little-endian format.
 *
 * @param[in]  buffer Pointer to a buffer containing the 4-byte serial number.
 * @param[in]  size Pointer to a variable holding the payload size on input.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   A pointer is NULL or the size is not 4.
 */
ctia_status_t cTIA_conf_serial_number(uint8_t *buffer, uint8_t *size);

/**
 * @brief  Configures the device's available MEAS channels.
 *
 * @param[in]  channel count uint8_t.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   count is > MAX_CH_COUNT.
 */
ctia_status_t cTIA_conf_available_meas_ch(uint8_t channel_count);

/**
 * @brief  Configures the device's available STIM channels.
 *
 * @param[in]  channel count uint8_t.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   count is > MAX_CH_COUNT.
 */
ctia_status_t cTIA_conf_available_stim_ch(uint8_t channel_count);

/**
 * @brief  Configures the device's available EXT STIM channels.
 *
 * @param[in]  channel count uint8_t.
 *
 * @retval CTIA_SUCCESS             Operation completed successfully.
 * @retval CTIA_INVALID_PARAMETER   count is > MAX_STIM_CH_COUNT.
 */
ctia_status_t cTIA_conf_available_ext_stim_ch(uint8_t channel_count);

/**
 * @brief  	Executes a relay matrix self-test. By switching the relays
 * 			in various configurations and testing if the analog bus is shorted or not.
 * 			This way the device can detect stuck relays within the matrix.
 *
 * @param[out] buffer 				Pointer to a buffer where the test result bitfields will be stored.
 * @param[out] size   				Pointer to a variable that receives the number of bytes
 *                    				written into @p buffer.
 *
 * @retval CTIA_SUCCESS             Self-test was completed
 * @retval CTIA_FAIL			   	Self-test was aborted
 */
ctia_status_t cTIA_execute_selftest(uint8_t *buffer, uint8_t *size);

ctia_status_t cTIA_uart_transmit(uint8_t *buffer, uint8_t size);

#endif /* CTIA_H_ */
