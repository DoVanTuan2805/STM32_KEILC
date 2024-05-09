/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define BT_DOWN_Pin GPIO_PIN_14
#define BT_DOWN_GPIO_Port GPIOC
#define BT_UP_Pin GPIO_PIN_15
#define BT_UP_GPIO_Port GPIOC
#define A1_Pin GPIO_PIN_0
#define A1_GPIO_Port GPIOA
#define A1_EXTI_IRQn EXTI0_IRQn
#define B1_Pin GPIO_PIN_1
#define B1_GPIO_Port GPIOA
#define EN1_Pin GPIO_PIN_2
#define EN1_GPIO_Port GPIOA
#define DIR1_Pin GPIO_PIN_3
#define DIR1_GPIO_Port GPIOA
#define PUL1_Pin GPIO_PIN_4
#define PUL1_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_0
#define CS_GPIO_Port GPIOB
#define RST_Pin GPIO_PIN_1
#define RST_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOB
#define LED_TFT_Pin GPIO_PIN_10
#define LED_TFT_GPIO_Port GPIOB
#define R4_Pin GPIO_PIN_12
#define R4_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_13
#define R3_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_14
#define R2_GPIO_Port GPIOB
#define R1_Pin GPIO_PIN_15
#define R1_GPIO_Port GPIOB
#define C4_Pin GPIO_PIN_8
#define C4_GPIO_Port GPIOA
#define C3_Pin GPIO_PIN_9
#define C3_GPIO_Port GPIOA
#define C2_Pin GPIO_PIN_10
#define C2_GPIO_Port GPIOA
#define C1_Pin GPIO_PIN_11
#define C1_GPIO_Port GPIOA
#define PUL2_Pin GPIO_PIN_12
#define PUL2_GPIO_Port GPIOA
#define DIR2_Pin GPIO_PIN_15
#define DIR2_GPIO_Port GPIOA
#define EN2_Pin GPIO_PIN_3
#define EN2_GPIO_Port GPIOB
#define B2_Pin GPIO_PIN_6
#define B2_GPIO_Port GPIOB
#define A2_Pin GPIO_PIN_7
#define A2_GPIO_Port GPIOB
#define A2_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
