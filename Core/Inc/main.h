/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EXT_PROBE_ON_Pin GPIO_PIN_0
#define EXT_PROBE_ON_GPIO_Port GPIOA
#define SHIFT_REG_CLR_H_Pin GPIO_PIN_4
#define SHIFT_REG_CLR_H_GPIO_Port GPIOA
#define SHIFT_REG_CLR_L_Pin GPIO_PIN_5
#define SHIFT_REG_CLR_L_GPIO_Port GPIOA
#define ANALOG_BUS_DETECT_INPUT_Pin GPIO_PIN_0
#define ANALOG_BUS_DETECT_INPUT_GPIO_Port GPIOB
#define SHIFT_REG_G_STIM_Pin GPIO_PIN_1
#define SHIFT_REG_G_STIM_GPIO_Port GPIOB
#define SHIFT_REG_CLR_STIM_Pin GPIO_PIN_2
#define SHIFT_REG_CLR_STIM_GPIO_Port GPIOB
#define SHIFT_REG_EN_MEAS_H_Pin GPIO_PIN_10
#define SHIFT_REG_EN_MEAS_H_GPIO_Port GPIOB
#define SHIFT_REG_EN_MEAS_L_Pin GPIO_PIN_11
#define SHIFT_REG_EN_MEAS_L_GPIO_Port GPIOB
#define SHIFT_REG_EN_STIM_Pin GPIO_PIN_12
#define SHIFT_REG_EN_STIM_GPIO_Port GPIOB
#define SHIFT_REG_G_MEAS_H_Pin GPIO_PIN_13
#define SHIFT_REG_G_MEAS_H_GPIO_Port GPIOB
#define SHIFT_REG_G_MEAS_L_Pin GPIO_PIN_14
#define SHIFT_REG_G_MEAS_L_GPIO_Port GPIOB
#define PROBE_DETECT_Pin GPIO_PIN_6
#define PROBE_DETECT_GPIO_Port GPIOC
#define ANALOG_BUS_DETECT_ENABLE_Pin GPIO_PIN_15
#define ANALOG_BUS_DETECT_ENABLE_GPIO_Port GPIOA
#define EXT_STIM_1_ON_Pin GPIO_PIN_0
#define EXT_STIM_1_ON_GPIO_Port GPIOD
#define EXT_STIM_2_ON_Pin GPIO_PIN_1
#define EXT_STIM_2_ON_GPIO_Port GPIOD
#define EXT_STIM_3_ON_Pin GPIO_PIN_2
#define EXT_STIM_3_ON_GPIO_Port GPIOD
#define EXT_STIM_4_ON_Pin GPIO_PIN_3
#define EXT_STIM_4_ON_GPIO_Port GPIOD
#define SHIFT_REG_SRCK_Pin GPIO_PIN_3
#define SHIFT_REG_SRCK_GPIO_Port GPIOB
#define SHIFT_REG_SER_IN_Pin GPIO_PIN_5
#define SHIFT_REG_SER_IN_GPIO_Port GPIOB
#define SHIFT_REG_RCK_MEAS_H_Pin GPIO_PIN_6
#define SHIFT_REG_RCK_MEAS_H_GPIO_Port GPIOB
#define SHIFT_REG_RCK_MEAS_L_Pin GPIO_PIN_7
#define SHIFT_REG_RCK_MEAS_L_GPIO_Port GPIOB
#define SHIFT_REG_RCK_STIM_Pin GPIO_PIN_8
#define SHIFT_REG_RCK_STIM_GPIO_Port GPIOB
#define SHIFT_REG_ERR_Pin GPIO_PIN_9
#define SHIFT_REG_ERR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define DEVICE_ID 0xA101
#define DEVICE_NAME_STRING "compact Test Interface Adapter"
#define FW_VERSION_STRING "0.4.2"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
