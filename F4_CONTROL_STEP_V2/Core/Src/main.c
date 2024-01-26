/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "encoder.h"
#include "Keypad_user.h"
#include "flash.h"
#include "Button_user.h"
#include "ILI9431_user.h"
#include "step_user.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "stdbool.h"
#include "logo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


dataUser_t dataUserInput;

volatile int16_t valueEncoder1, valueEncoder2;
volatile bool changeEncoder1 ,changeEncoder2;
extern bool changeButton;
Screen_t screenCurr = ENCODER_ROTATION_SCREEN, screenPrev = ENCODER_ANGLE_SCREEN;

uint64_t timeLed, timePress;

char keyPressCurr, keyPressLast;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance == htim11.Instance)
		{
				handleKeypadInTimer();
				keyPressCurr = getKey();
				
				if(keyPressLast != keyPressCurr)
				{
					keyPressLast = keyPressCurr;
					if(keyPressCurr == 'A')							// ON/OFF MANUAL ANGLE
					{
							if(screenCurr == ENCODER_ANGLE_SCREEN)
							{
									screenCurr = HAND_ANGLE_SCREEN;
							}
							else if(screenCurr == HAND_ANGLE_SCREEN)
							{
									screenCurr = ENCODER_ANGLE_SCREEN;
							}
							else
							{
									screenCurr = ENCODER_ANGLE_SCREEN;
							}
					}
					else if (keyPressCurr == 'B')				// ON/OFF MANUAL ROTATION
					{
							if(screenCurr == ENCODER_ROTATION_SCREEN)
							{
									screenCurr = HAND_ROTATION_SCREEN;
							}
							else if(screenCurr == HAND_ROTATION_SCREEN)
							{
									screenCurr = ENCODER_ROTATION_SCREEN;
							}
							else
							{
									screenCurr = ENCODER_ROTATION_SCREEN;
							}
					}
					else if (keyPressCurr == 'C')				// ON/OFF CHEATER
					{
					}
					else if (keyPressCurr == 'D')				// SET-UP
					{
							screenCurr = SETUP_SCREEN;
					}
			}
		}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if(GPIO_Pin == A1_Pin)
		{
				handleInterruptEncoder(GPIO_Pin, A1_GPIO_Port, A1_Pin, B1_GPIO_Port, B1_Pin);
		}	
		else if(GPIO_Pin == A2_Pin)
		{
				handleInterruptEncoder(GPIO_Pin, A2_GPIO_Port, A2_Pin, B2_GPIO_Port, B2_Pin);
		}
}
void increse_callback(uint16_t GPIO_Pin)
{
		if(GPIO_Pin == A1_Pin)				// 	ENCODER 1
		{
				changeEncoder1 = true;
				if(screenCurr == ENCODER_ANGLE_SCREEN)
				{
						dataUserInput.AngleInTFT++;
						if(dataUserInput.AngleInTFT >= dataUserInput.IndexAngle)
						{
								dataUserInput.AngleInTFT = 0;
						}
						else if(dataUserInput.AngleInTFT <= 0)
						{
								dataUserInput.AngleInTFT = 0;
						}
				}
				else if(screenCurr == HAND_ANGLE_SCREEN)
				{
						dataUserInput.indexSlotInAngle++;
						if(dataUserInput.indexSlotInAngle > 3)
						{
								dataUserInput.indexSlotInAngle = 1;
								dataUserInput.indexPageInAngle++;
						}
						else if(dataUserInput.indexSlotInAngle < 1)
						{
								dataUserInput.indexSlotInAngle = 3;
								dataUserInput.indexPageInAngle--;
						}
						if(dataUserInput.indexPageInAngle < 1)
						{
								dataUserInput.indexPageInAngle = 10;
						}
						if(dataUserInput.indexPageInAngle > 10)
						{
								dataUserInput.indexPageInAngle = 1;
						}
				}
				else if(screenCurr == SETUP_SCREEN)
				{
						dataUserInput.indexSlotInSetup++;
						if(dataUserInput.indexSlotInSetup > 3)
						{
								dataUserInput.indexSlotInSetup = 1;
								dataUserInput.indexPageInSetup++;
						}
						else if(dataUserInput.indexSlotInSetup < 1)
						{
								dataUserInput.indexSlotInSetup = 3;
								dataUserInput.indexPageInSetup--;
						}
						if(dataUserInput.indexPageInSetup < 1)
						{
								dataUserInput.indexPageInSetup = 2;
						}
						if(dataUserInput.indexPageInSetup > 2)
						{
								dataUserInput.indexPageInSetup = 1;
						}
				}
		}
		else if(GPIO_Pin == A2_Pin)		// 	ENCODER 2
		{	
				changeEncoder2 = true;
				if(screenCurr == ENCODER_ROTATION_SCREEN)
				{
						dataUserInput.RotationInTFT += ROUND / dataUserInput.IndexRotation;				// 32 -> 11.25
						if(dataUserInput.RotationInTFT >= ROUND )
						{
								dataUserInput.RotationInTFT = 0;
						}
						else if(dataUserInput.RotationInTFT <= 0)
						{
								dataUserInput.RotationInTFT = 0;
						}
				}
				else if(screenCurr == HAND_ROTATION_SCREEN)
				{
						dataUserInput.indexSlotInRotation++;
						if(dataUserInput.indexSlotInRotation > 3)
						{
								dataUserInput.indexSlotInRotation = 1;
								dataUserInput.indexPageInRotation++;
						}
						else if(dataUserInput.indexSlotInRotation < 1)
						{
								dataUserInput.indexSlotInRotation = 3;
								dataUserInput.indexPageInRotation--;
						}
				}
				
		}
}
void decrese_callback(uint16_t GPIO_Pin)		
{
		if(GPIO_Pin == A1_Pin)				// 	ENCODER 1
		{
				changeEncoder1 = true;
				if(screenCurr == ENCODER_ANGLE_SCREEN)
				{
						dataUserInput.AngleInTFT--;
						if(dataUserInput.AngleInTFT >= dataUserInput.IndexAngle)
						{
								dataUserInput.AngleInTFT = 0;
						}
						else if(dataUserInput.AngleInTFT <= 0)
						{
								dataUserInput.AngleInTFT = 0;
						}
				}
				else if(screenCurr == HAND_ANGLE_SCREEN)
				{
						dataUserInput.indexSlotInAngle--;
						if(dataUserInput.indexSlotInAngle > 3)
						{
								dataUserInput.indexSlotInAngle = 1;
								dataUserInput.indexPageInAngle++;
						}
						else if(dataUserInput.indexSlotInAngle < 1)
						{
								dataUserInput.indexSlotInAngle = 3;
								dataUserInput.indexPageInAngle--;
						}
						if(dataUserInput.indexPageInAngle < 1)
						{
								dataUserInput.indexPageInAngle = 10;
						}
						if(dataUserInput.indexPageInAngle > 10)
						{
								dataUserInput.indexPageInAngle = 1;
						}
				}
				
				else if(screenCurr == SETUP_SCREEN)
				{
						dataUserInput.indexSlotInSetup--;
						if(dataUserInput.indexSlotInSetup > 3)
						{
								dataUserInput.indexSlotInSetup = 1;
								dataUserInput.indexPageInSetup++;
						}
						else if(dataUserInput.indexSlotInSetup < 1)
						{
								dataUserInput.indexSlotInSetup = 3;
								dataUserInput.indexPageInSetup--;
						}
						if(dataUserInput.indexPageInSetup < 1)
						{
								dataUserInput.indexPageInSetup = 2;
						}
						if(dataUserInput.indexPageInSetup > 2)
						{
								dataUserInput.indexPageInSetup = 1;
						}
				}
		}
		else if(GPIO_Pin == A2_Pin)		// 	ENCODER 2
		{
				changeEncoder2 = true;
				if(screenCurr == ENCODER_ROTATION_SCREEN)
				{
						dataUserInput.RotationInTFT -= ROUND / dataUserInput.IndexRotation;
						if(dataUserInput.RotationInTFT >= ROUND)
						{
								dataUserInput.RotationInTFT = 0;
						}
						else if(dataUserInput.RotationInTFT < 0)
						{
								dataUserInput.AngleInTFT = ROUND;
						}
						
				}
				else if(screenCurr == HAND_ROTATION_SCREEN)
				{
						dataUserInput.indexSlotInRotation--;
						if(dataUserInput.indexSlotInRotation > 3)
						{
								dataUserInput.indexSlotInRotation = 1;
								dataUserInput.indexPageInRotation++;
						}
						else if(dataUserInput.indexSlotInRotation < 1)
						{
								dataUserInput.indexSlotInRotation = 3;
								dataUserInput.indexPageInRotation--;
						}
						if(dataUserInput.indexPageInRotation < 1)
						{
								dataUserInput.indexPageInRotation = 10;
						}
						if(dataUserInput.indexPageInRotation > 10)
						{
								dataUserInput.indexPageInRotation = 1;
						}
				}
				
		}
		
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
	dataUserInput.IndexRotation = 32;					//(32, 64, 96)
	dataUserInput.IndexAngle = 360;
	dataUserInput.indexPageInAngle = 1;
	dataUserInput.indexPageInRotation= 1;
	dataUserInput.indexSlotInAngle = 1;
	dataUserInput.indexSlotInRotation = 1;
	dataUserInput.indexPageInSetup = 1;
	dataUserInput.indexSlotInSetup = 1;
	initILI();
	initButton();
	initKeypad();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		buttonHandle();
		BlynkSlot(dataUserInput);
		if(HAL_GetTick() - timeLed > 100)
		{
				if(screenPrev != screenCurr)
				{
						screenPrev = screenCurr;
						clearLCD();
						ArrayTFT(screenCurr, dataUserInput);
				}
				if(changeEncoder1 == true || changeEncoder2 == true || changeButton == true)
				{
					changeEncoder1 = false;
					changeEncoder2 = false;
					changeButton = false;
					ArrayTFT(screenCurr, dataUserInput);
				}
				
				timeLed = HAL_GetTick();
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
