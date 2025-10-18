/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EXT_PROBE_ON_Pin|EXT_TRIG_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SHIFT_REG_G_STIM_Pin|SHIFT_REG_CLR_Pin|SHIFT_REG_EN_MEAS_H_Pin|SHIFT_REG_EN_MEAS_L_Pin
                          |SHIFT_REG_EN_STIM_Pin|SHIFT_REG_G_MEAS_H_Pin|SHIFT_REG_G_MEAS_L_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, EXT_STIM_1_ON_Pin|EXT_STIM_2_ON_Pin|EXT_STIM_3_ON_Pin|EXT_STIM_4_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SHIFT_REG_RCK_MEAS_H_Pin|SHIFT_REG_RCK_MEAS_L_Pin|SHIFT_REG_RCK_STIM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : EXT_PROBE_ON_Pin EXT_TRIG_ON_Pin */
  GPIO_InitStruct.Pin = EXT_PROBE_ON_Pin|EXT_TRIG_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : EXT_TRIG_INT_Pin */
  GPIO_InitStruct.Pin = EXT_TRIG_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EXT_TRIG_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SHIFT_REG_G_STIM_Pin SHIFT_REG_CLR_Pin SHIFT_REG_EN_MEAS_H_Pin SHIFT_REG_EN_MEAS_L_Pin
                           SHIFT_REG_EN_STIM_Pin SHIFT_REG_G_MEAS_H_Pin SHIFT_REG_G_MEAS_L_Pin SHIFT_REG_RCK_MEAS_H_Pin
                           SHIFT_REG_RCK_MEAS_L_Pin SHIFT_REG_RCK_STIM_Pin */
  GPIO_InitStruct.Pin = SHIFT_REG_G_STIM_Pin|SHIFT_REG_CLR_Pin|SHIFT_REG_EN_MEAS_H_Pin|SHIFT_REG_EN_MEAS_L_Pin
                          |SHIFT_REG_EN_STIM_Pin|SHIFT_REG_G_MEAS_H_Pin|SHIFT_REG_G_MEAS_L_Pin|SHIFT_REG_RCK_MEAS_H_Pin
                          |SHIFT_REG_RCK_MEAS_L_Pin|SHIFT_REG_RCK_STIM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PROBE_BTN_INT_Pin */
  GPIO_InitStruct.Pin = PROBE_BTN_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PROBE_BTN_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : EXT_STIM_1_ON_Pin EXT_STIM_2_ON_Pin EXT_STIM_3_ON_Pin EXT_STIM_4_ON_Pin */
  GPIO_InitStruct.Pin = EXT_STIM_1_ON_Pin|EXT_STIM_2_ON_Pin|EXT_STIM_3_ON_Pin|EXT_STIM_4_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : SHIFT_REG_ERR_Pin */
  GPIO_InitStruct.Pin = SHIFT_REG_ERR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SHIFT_REG_ERR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
