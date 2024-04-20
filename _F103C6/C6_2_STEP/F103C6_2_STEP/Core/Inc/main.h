/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define DIR_Z_Pin GPIO_PIN_11
#define DIR_Z_GPIO_Port GPIOA
#define STEP_Z_Pin GPIO_PIN_12
#define STEP_Z_GPIO_Port GPIOA
#define MS3_Z_Pin GPIO_PIN_15
#define MS3_Z_GPIO_Port GPIOA
#define MS2_Z_Pin GPIO_PIN_3
#define MS2_Z_GPIO_Port GPIOB
#define MS1_Z_Pin GPIO_PIN_4
#define MS1_Z_GPIO_Port GPIOB
#define DIR_Y_Pin GPIO_PIN_5
#define DIR_Y_GPIO_Port GPIOB
#define STEP_Y_Pin GPIO_PIN_6
#define STEP_Y_GPIO_Port GPIOB
#define MS3_Y_Pin GPIO_PIN_7
#define MS3_Y_GPIO_Port GPIOB
#define MS2_Y_Pin GPIO_PIN_8
#define MS2_Y_GPIO_Port GPIOB
#define MS1_Y_Pin GPIO_PIN_9
#define MS1_Y_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
