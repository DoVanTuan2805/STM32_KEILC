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
#include "mainTFT.h"
#include "Flash_user.h"
#include "user_ex.h"
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
extern float ROUND;

dataUser_t dataUserInput;

volatile int16_t valueEncoder1, valueEncoder2;
volatile bool changeEncoder1, changeEncoder2;
extern bool changeButton;
Screen_t screenCurr = ENCODER_ROTATION_SCREEN, screenPrev = HAND_ROTATION_SCREEN;

uint64_t timeLed, timePress;

char keyPressCurr, keyPressLast;

dataSaveFlash_t dataWriteFlash, dataReadFlash;
bool stateSaveSetup;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if (htim->Instance == htim11.Instance)
	{
		handleKeypadInTimer();
		keyPressCurr = getKey();
		if (keyPressLast != keyPressCurr)
		{

			keyPressLast = keyPressCurr;
			{
				// SET UP SCREEN LOOP MODE, PULSE , SPEED
				if (screenCurr == SETUP_SCREEN)
				{
					changeButton = true;
					if (dataUserInput.indexPageInSetup == 1)
					{
						// SET UP LOOP MODE
						if (dataUserInput.indexSlotInSetup == 1)
						{
							if (keyPressCurr == '#')
							{

								if (dataUserInput.dataSetup.LoopMode == false)
								{
									dataUserInput.dataSetup.LoopMode = true;
								}
								else if (dataUserInput.dataSetup.LoopMode == true)
								{
									dataUserInput.dataSetup.LoopMode = false;
								}
							}
						}
						// SET UP PULSE
						else if (dataUserInput.indexSlotInSetup == 2)
						{
							// PRESS NUMBER PULSE
							if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
							{
								dataUserInput.dataSetup.changePulse = true;
								if (keyPressCurr == '*')
								{
									keyPressCurr = '.';
								}
								if (dataUserInput.dataSetup.indexArrayPulse > sizeof(dataUserInput.dataSetup.arrayPulse))
								{
									dataUserInput.dataSetup.indexArrayPulse = 0;
								}
								dataUserInput.dataSetup.arrayPulse[dataUserInput.dataSetup.indexArrayPulse++] = (char)keyPressCurr;
							}
							// COMFIRM PULSE -> SAVE TO FLASH
							else if (keyPressCurr == '#')
							{
								dataUserInput.dataSetup.changePulse = false;
								dataUserInput.dataSetup.indexArrayPulse = 0;
								dataUserInput.dataSetup.pulse = atoi(dataUserInput.dataSetup.arrayPulse);
								sprintf(dataWriteFlash.Pulse, "%llu", dataUserInput.dataSetup.pulse);
								stateSaveSetup = true; // START SAVE SETUP
							}
						}
						// SET UP SPEED
						else if (dataUserInput.indexSlotInSetup == 3)
						{
							// PRESS NUMBER SPEED
							if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
							{
								dataUserInput.dataSetup.changeSpeed = true;
								if (keyPressCurr == '*')
								{
									keyPressCurr = '.';
								}
								if (dataUserInput.dataSetup.indexArraySpeed < sizeof(dataUserInput.dataSetup.arraySpeed))
								{
									dataUserInput.dataSetup.arraySpeed[dataUserInput.dataSetup.indexArraySpeed++] = (char)keyPressCurr;
								}
								else
								{
									dataUserInput.dataSetup.indexArraySpeed = 0;
								}
							}
							// COMFIRM SPEED -> SAVE TO FLASH
							else if (keyPressCurr == '#')
							{
								dataUserInput.dataSetup.changeSpeed = false;
								dataUserInput.dataSetup.indexArraySpeed = 0;
								dataUserInput.dataSetup.speed = atoi(dataUserInput.dataSetup.arraySpeed);
								if (dataUserInput.dataSetup.speed > MAX_SPEED)
								{
									dataUserInput.dataSetup.speed = MAX_SPEED;
								}
								sprintf(dataWriteFlash.Speed, "%d", dataUserInput.dataSetup.speed);
								stateSaveSetup = true; // START SAVE SETUP
							}
						}
					}
				}
				// SELECT SCREEN CREATE OR MODIFY
				if (screenCurr == SELECT_CREAT_MODIFY_SCREEN)
				{
					if (keyPressCurr == '#')
					{
						if (dataUserInput.indexCreateModify == 1)
						{
						}
						else if (dataUserInput.indexCreateModify == 2)
						{
							screenCurr = CREATE_ROTATION_SCREEEN;
						}
					}
				}
				// SELECT SCREEN CREATE
				if (screenCurr == CREATE_ROTATION_SCREEEN)
				{
					if (dataUserInput.indexCreate == 1)
					{
						changeButton = true;
						// PRESS NUMBER PAGE
						if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
						{
							dataUserInput.dataSetupRotation.changeCreatPage = true;
							if (keyPressCurr == '*')
							{
								keyPressCurr = '.';
							}
							if (dataUserInput.dataSetupRotation.indexStrPage < sizeof(dataUserInput.dataSetupRotation.strPage))
							{
								dataUserInput.dataSetupRotation.strPage[dataUserInput.dataSetupRotation.indexStrPage++] = (char)keyPressCurr;
							}
							else
							{
								dataUserInput.dataSetupRotation.indexStrPage = 0;
							}
						}
						// COMFIRM PAGE -> SAVE TO FLASH
						else if (keyPressCurr == '#')
						{
							dataUserInput.dataSetupRotation.nPage = atoi(dataUserInput.dataSetupRotation.strPage);
							dataUserInput.dataSetupRotation.changeCreatPage = false;
							dataUserInput.dataSetupRotation.indexStrPage = 0;
						}
					}
					else if (dataUserInput.indexCreate == 2)
					{
						changeButton = true;
						// PRESS NUMBER PAGE
						if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
						{
							dataUserInput.dataSetupRotation.changeCreatPage = true;
							if (keyPressCurr == '*')
							{
								keyPressCurr = '.';
							}
							if (dataUserInput.dataSetupRotation.indexStrSlot < sizeof(dataUserInput.dataSetupRotation.strSlot))
							{
								dataUserInput.dataSetupRotation.strSlot[dataUserInput.dataSetupRotation.indexStrSlot++] = (char)keyPressCurr;
							}
							else
							{
								dataUserInput.dataSetupRotation.indexStrSlot = 0;
							}
						}
						// COMFIRM PAGE -> SAVE TO FLASH
						else if (keyPressCurr == '#')
						{
							dataUserInput.dataSetupRotation.nSlot = atoi(dataUserInput.dataSetupRotation.strSlot);
							dataUserInput.dataSetupRotation.changeCreatPage = false;
							dataUserInput.dataSetupRotation.indexStrSlot = 0;
						}
					}
				}
				if (screenCurr == HAND_ROTATION_SCREEN)
				{
								}
			}
			{
				if (keyPressCurr == 'A') // ON/OFF MANUAL ANGLE
				{
					if (screenCurr != ANGLE_SETUP_SCREEN && screenCurr != ANGLE_SETUP_SCREEN && screenCurr != HAND_ROTATION_SCREEN && screenCurr != SETUP_SCREEN)
					{
						if (screenCurr == ENCODER_ROTATION_SCREEN)
						{
							screenCurr = FLOOR_MAIN_SCREEN;
						}
						else if (screenCurr == CHEATER_SCREEN)
						{
							screenCurr = FLOOR_MAIN_SCREEN;
						}
						else if (screenCurr == FLOOR_MAIN_SCREEN)
						{
							screenCurr = ANGLE_MAIN_SCREEN;
						}
						else if (screenCurr == ANGLE_MAIN_SCREEN)
						{
							screenCurr = FLOOR_MAIN_SCREEN;
						}
					}
				}
				else if (keyPressCurr == 'B') // ON/OFF MANUAL ROTATION
				{
					if (screenCurr == ENCODER_ROTATION_SCREEN)
					{
						screenCurr = SELECT_CREAT_MODIFY_SCREEN;
					}
					else if (screenCurr == SELECT_CREAT_MODIFY_SCREEN)
					{
						screenCurr = ENCODER_ROTATION_SCREEN;
					}
					else
					{
						screenCurr = ENCODER_ROTATION_SCREEN;
					}
				}
				else if (keyPressCurr == 'C') // ON/OFF CHEATER
				{
					if (screenCurr != ANGLE_SETUP_SCREEN && screenCurr != ANGLE_SETUP_SCREEN && screenCurr != HAND_ROTATION_SCREEN && screenCurr != SETUP_SCREEN)
					{
						if (screenCurr == ENCODER_ROTATION_SCREEN)
						{
							screenCurr = CHEATER_SCREEN;
						}
						else if (screenCurr == CHEATER_SCREEN)
						{
							screenCurr = ENCODER_ROTATION_SCREEN;
						}
						else if (screenCurr == ANGLE_MAIN_SCREEN || screenCurr == FLOOR_MAIN_SCREEN)
						{
							screenCurr = CHEATER_SCREEN;
						}
					}
				}
				else if (keyPressCurr == 'D') // SET-UP
				{
					screenCurr = SETUP_SCREEN;
				}
			}
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == A1_Pin)
	{
		handleInterruptEncoder(GPIO_Pin, A1_GPIO_Port, A1_Pin, B1_GPIO_Port, B1_Pin);
	}
	else if (GPIO_Pin == A2_Pin)
	{
		handleInterruptEncoder(GPIO_Pin, A2_GPIO_Port, A2_Pin, B2_GPIO_Port, B2_Pin);
	}
}
void increse_callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == A1_Pin) // 	ENCODER LEFT
	{
		changeEncoder1 = true;
		if (screenCurr == ENCODER_ROTATION_SCREEN)
		{
			dataUserInput.RotationInTFT += ROUND / dataUserInput.IndexRotation; // 32 -> 11.25
			if (dataUserInput.RotationInTFT > ROUND)
			{
				dataUserInput.RotationInTFT = ROUND / dataUserInput.IndexRotation;
			}
			else if (dataUserInput.RotationInTFT <= 0)
			{
				dataUserInput.RotationInTFT = ROUND;
			}
		}
		else if (screenCurr == SETUP_SCREEN)
		{
			dataUserInput.indexSlotInSetup++;
			if (dataUserInput.indexSlotInSetup > 3)
			{
				dataUserInput.indexSlotInSetup = 1;
				dataUserInput.indexPageInSetup++;
			}
			else if (dataUserInput.indexSlotInSetup < 1)
			{
				dataUserInput.indexSlotInSetup = 3;
				dataUserInput.indexPageInSetup--;
			}
			if (dataUserInput.indexPageInSetup < 1)
			{
				dataUserInput.indexPageInSetup = 2;
			}
			if (dataUserInput.indexPageInSetup > 2)
			{
				dataUserInput.indexPageInSetup = 1;
			}
		}
	}
	else if (GPIO_Pin == A2_Pin) // 	ENCODER RIGHT
	{
		changeEncoder2 = true;
		if (screenCurr == HAND_ROTATION_SCREEN)
		{
			dataUserInput.dataSetupRotation.indexSlotInRotation++;
			if (dataUserInput.dataSetupRotation.indexSlotInRotation > 3)
			{
				dataUserInput.dataSetupRotation.indexSlotInRotation = 1;
				dataUserInput.dataSetupRotation.indexPageInRotation++;
			}
			if (dataUserInput.dataSetupRotation.indexPageInRotation > (dataUserInput.dataSetupRotation.nSlot / 3))
			{
				dataUserInput.dataSetupRotation.indexPageInRotation = 1;
			}
		}
		else if (screenCurr == CHEATER_SCREEN)
		{
			dataUserInput.fCheater += 0.1;
		}
		else if (screenCurr == SELECT_CREAT_MODIFY_SCREEN)
		{
			dataUserInput.indexCreateModify++;
			if (dataUserInput.indexCreateModify > 2)
			{
				dataUserInput.indexCreateModify = 1;
			}
		}
		else if (screenCurr == CREATE_ROTATION_SCREEEN)
		{
			dataUserInput.indexCreate++;
			if (dataUserInput.indexCreate > 2)
			{
				dataUserInput.indexCreate = 1;
			}
		}
	}
}
void decrese_callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == A1_Pin) // 	ENCODER LEFT
	{
		changeEncoder1 = true;
		if (screenCurr == ENCODER_ROTATION_SCREEN)
		{
			dataUserInput.RotationInTFT -= ROUND / dataUserInput.IndexRotation; // 32 -> 11.25
			if (dataUserInput.RotationInTFT > ROUND)
			{
				dataUserInput.RotationInTFT = ROUND / dataUserInput.IndexRotation;
			}
			else if (dataUserInput.RotationInTFT <= 0)
			{
				dataUserInput.RotationInTFT = ROUND;
			}
		}
		else if (screenCurr == SETUP_SCREEN)
		{
			dataUserInput.indexSlotInSetup--;
			if (dataUserInput.indexSlotInSetup > 3)
			{
				dataUserInput.indexSlotInSetup = 1;
				dataUserInput.indexPageInSetup++;
			}
			else if (dataUserInput.indexSlotInSetup < 1)
			{
				dataUserInput.indexSlotInSetup = 3;
				dataUserInput.indexPageInSetup--;
			}
			if (dataUserInput.indexPageInSetup < 1)
			{
				dataUserInput.indexPageInSetup = 2;
			}
			if (dataUserInput.indexPageInSetup > 2)
			{
				dataUserInput.indexPageInSetup = 1;
			}
		}
	}
	else if (GPIO_Pin == A2_Pin) // 	ENCODER RIGHT
	{
		changeEncoder2 = true;
		if (screenCurr == HAND_ROTATION_SCREEN)
		{
			dataUserInput.dataSetupRotation.indexSlotInRotation--;
			if (dataUserInput.dataSetupRotation.indexSlotInRotation < 1)
			{
				dataUserInput.dataSetupRotation.indexSlotInRotation = 3;
				dataUserInput.dataSetupRotation.indexPageInRotation--;
			}
			if (dataUserInput.dataSetupRotation.indexPageInRotation < 1)
			{
				dataUserInput.dataSetupRotation.indexPageInRotation = (dataUserInput.dataSetupRotation.nSlot / 3);
			}
		}
		else if (screenCurr == CHEATER_SCREEN)
		{
			dataUserInput.fCheater -= 0.1;
		}
		else if (screenCurr == SELECT_CREAT_MODIFY_SCREEN)
		{
			dataUserInput.indexCreateModify--;
			if (dataUserInput.indexCreateModify < 1)
			{
				dataUserInput.indexCreateModify = 2;
			}
		}
		else if (screenCurr == CREATE_ROTATION_SCREEEN)
		{
			dataUserInput.indexCreate--;
			if (dataUserInput.indexCreate < 1)
			{
				dataUserInput.indexCreate = 2;
			}
		}
	}
}

uint8_t sizeArraypage;
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
	dataUserInput.IndexRotation = 32;	 //(32, 64, 96)
	dataUserInput.RotationInTFT = ROUND; // 360

	dataUserInput.indexFloorInRotation = 1;
	dataUserInput.indexPageInSetup = 1;
	dataUserInput.indexSlotInSetup = 1;
	dataUserInput.fCheater = 0.0;

	// DEFINE ANGLE NUMBER IN MAIN
	dataUserInput.dataAngleInMain.rotation_1 = 2.0;
	dataUserInput.dataAngleInMain.rotation_2 = 4.5;
	dataUserInput.dataAngleInMain.rotation_3 = 19.5;
	dataUserInput.dataAngleInMain.rotation_4 = 190.5;
	dataUserInput.dataAngleInMain.rotation_5 = 340.5;

	dataUserInput.dataSetup.LoopMode = true;
	// DEFINE SPEED
	dataUserInput.dataSetup.speed = 100;
	memset(dataUserInput.dataSetup.arraySpeed, '0', sizeof(dataUserInput.dataSetup.arraySpeed));
	dataUserInput.dataSetup.indexArraySpeed = 0;
	dataUserInput.dataSetup.changeSpeed = false;

	// DEFINE PULSE
	dataUserInput.dataSetup.pulse = 9000;
	memset(dataUserInput.dataSetup.arrayPulse, '0', sizeof(dataUserInput.dataSetup.arrayPulse));
	dataUserInput.dataSetup.indexArrayPulse = 0;
	dataUserInput.dataSetup.changePulse = false;

	// DEFINE SELECT CREATE OR MODIFY SCREEN
	dataUserInput.dataSetupRotation.indexPageInRotation = 1;
	dataUserInput.dataSetupRotation.indexSlotInRotation = 1;

	dataUserInput.indexCreateModify = 1;
	dataUserInput.indexCreate = 1;
	memset(dataUserInput.dataSetupRotation.strPage, '0', sizeof(dataUserInput.dataSetupRotation.strPage));
	sizeArraypage = sizeof(dataUserInput.dataSetupRotation.strPage);
	dataUserInput.indexModify = 1;
	memset(dataUserInput.dataSetupRotation.strSlot, '0', sizeof(dataUserInput.dataSetupRotation.strSlot));

	Flash_Read_Struct(ADDRESS_STORAGE, &dataReadFlash, sizeof(dataReadFlash));
	HAL_Delay(50);
	dataUserInput.dataSetup.speed = atoi(dataReadFlash.Speed);
	dataUserInput.dataSetup.pulse = atoi(dataReadFlash.Pulse);

	initILI();
	initButton();
	initKeypad();

	ILI9341_Draw_Image((const char *)layoutMain, SCREEN_HORIZONTAL_2);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		// dataSetupRotation.page = malloc(10 * sizeof(dataSetupRotation.page));
		buttonHandle();
		BlynkSlot(dataUserInput);
		if (HAL_GetTick() - timeLed > 100)
		{
			if (stateSaveSetup == true)
			{
				stateSaveSetup = false;
				Flash_Write_Struct(ADDRESS_STORAGE, dataWriteFlash);
			}
			if (screenPrev != screenCurr)
			{
				screenPrev = screenCurr;
				clearLCD();
				if (screenCurr == ENCODER_ROTATION_SCREEN || screenCurr == CHEATER_SCREEN || screenCurr == FLOOR_MAIN_SCREEN || screenCurr == ANGLE_MAIN_SCREEN)
				{
					ILI9341_Draw_Image((const char *)layoutMain, SCREEN_HORIZONTAL_2);
				}
				ArrayTFT(screenCurr, dataUserInput);
			}
			if (changeEncoder1 == true || changeEncoder2 == true || changeButton == true)
			{
				changeEncoder1 = false;
				changeEncoder2 = false;
				changeButton = false;
				ArrayTFT(screenCurr, dataUserInput);
			}
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

#ifdef USE_FULL_ASSERT
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
