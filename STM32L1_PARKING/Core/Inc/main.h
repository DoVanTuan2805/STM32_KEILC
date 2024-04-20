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
#include "stm32l1xx_hal.h"

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define PUL1_Pin GPIO_PIN_7
#define PUL1_GPIO_Port GPIOC
#define CTHT_HOME_Pin GPIO_PIN_9
#define CTHT_HOME_GPIO_Port GPIOC
#define PUL2_Pin GPIO_PIN_8
#define PUL2_GPIO_Port GPIOA
#define DIR2_Pin GPIO_PIN_9
#define DIR2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define IN1_Pin GPIO_PIN_2
#define IN1_GPIO_Port GPIOD
#define IN2_Pin GPIO_PIN_3
#define IN2_GPIO_Port GPIOB
#define EXTI_IN_A_Pin GPIO_PIN_4
#define EXTI_IN_A_GPIO_Port GPIOB
#define EXTI_IN_A_EXTI_IRQn EXTI4_IRQn
#define IN_B_Pin GPIO_PIN_5
#define IN_B_GPIO_Port GPIOB
#define DIR1_Pin GPIO_PIN_6
#define DIR1_GPIO_Port GPIOB
#define TC_HOME_Pin GPIO_PIN_7
#define TC_HOME_GPIO_Port GPIOB
#define TC_HOME_EXTI_IRQn EXTI9_5_IRQn
#define CTHT_PLATE_OUT_Pin GPIO_PIN_8
#define CTHT_PLATE_OUT_GPIO_Port GPIOB
#define CTHT_PLATE_IN_Pin GPIO_PIN_9
#define CTHT_PLATE_IN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
