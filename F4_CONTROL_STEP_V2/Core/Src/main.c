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
#include "saveToFlash.h"
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
extern uint8_t u8COLUMS;
extern bool changeButton;

uint8_t *arrayRotationW, *arrayRotationR;

dataUser_t dataUserInput;
dataSaveFlash_t dataWriteFlash, dataReadFlash;
dataPage_t dataPageW;

volatile bool changeEncoder1, changeEncoder2;

Screen_t screenCurr = ENCODER_ROTATION_SCREEN, screenPrev = ANGLE_MAIN_SCREEN;

volatile uint64_t timeLed, timeKeyPad;

char keyPressCurr, keyPressLast;
bool stateSaveSetup, stateSaveRotation;

uint8_t arrayTest[5];

volatile uint32_t pulseStep;
volatile bool isRun;
bool check; // FOR SCREEN LOOP MODE
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim9.Instance) // 100ms
	{
	}
	if (htim->Instance == htim10.Instance)
	{
		{
			if (dataUserInput.dataSetup.LoopMode == true)
			{
				if (pulseStep == 0)
				{
					pulseStep = dataUserInput.dataSetup.pulse;
				}
			}
		}
		{
			if (screenCurr != SETUP_SCREEN)
			{
				dataUserInput.dataSetup.LoopMode = false;
			}
		}
		if (pulseStep != 0)
		{
			isRun = true;
			pulseStep--;
			HAL_GPIO_TogglePin(PUL2_GPIO_Port, PUL2_Pin);
		}
		else if (pulseStep == 0)
		{
			isRun = false;
		}
	}
	if (htim->Instance == htim11.Instance)
	{
		handleKeypadInTimer();
		keyPressCurr = getKey();
		if (keyPressLast != keyPressCurr)
		{
			if (HAL_GetTick() - timeKeyPad > 50)
			{
				keyPressLast = keyPressCurr;
				{
					if (screenCurr == ENCODER_ROTATION_SCREEN)
					{
						changeButton = true;
						if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
						{
							dataUserInput.bKeypadEncoder = true;
							if (keyPressCurr == '*')
							{
								keyPressCurr = '.';
							}
							if (dataUserInput.indexRotEn < 2)
							{
								dataUserInput.arrayRotEn[dataUserInput.indexRotEn] = (char)keyPressCurr;
								dataUserInput.indexRotEn++;
							}
							else
							{
								dataUserInput.indexRotEn = 0;
							}
						}
						else if (keyPressCurr == '#')
						{
							uint8_t rot = atoi(dataUserInput.arrayRotEn);
							if (rot > dataUserInput.IndexRotation)
							{
								rot = dataUserInput.IndexRotation;
							}
							dataUserInput.RotationInTFT = rot * (ROUND / dataUserInput.IndexRotation);
							dataUserInput.bKeypadEncoder = false;
							memset(dataUserInput.arrayRotEn, '\0', sizeof(dataUserInput.arrayRotEn));
							dataUserInput.indexRotEn = 0;
						}
					}
					// SET UP SCREEN LOOP MODE, PULSE , SPEED
					else if (screenCurr == SETUP_SCREEN)
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
										check = false;
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
									dataUserInput.dataSetup.arrayPulse[dataUserInput.dataSetup.indexArrayPulse] = (char)keyPressCurr;
									dataUserInput.dataSetup.indexArrayPulse++;
								}
								// COMFIRM PULSE -> SAVE TO FLASH
								else if (keyPressCurr == '#')
								{
									dataUserInput.dataSetup.changePulse = false;
									dataUserInput.dataSetup.indexArrayPulse = 0;
									dataUserInput.dataSetup.pulse = atoi(dataUserInput.dataSetup.arrayPulse);
									// sscanf(dataUserInput.dataSetup.arrayPulse, "%lld", &dataUserInput.dataSetup.pulse);
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
									if (dataUserInput.dataSetup.indexArraySpeed > sizeof(dataUserInput.dataSetup.arraySpeed))
									{
										dataUserInput.dataSetup.indexArraySpeed = 0;
									}
									dataUserInput.dataSetup.arraySpeed[dataUserInput.dataSetup.indexArraySpeed] = (char)keyPressCurr;
									dataUserInput.dataSetup.indexArraySpeed++;
								}
								// COMFIRM SPEED -> SAVE TO FLASH
								else if (keyPressCurr == '#')
								{
									dataUserInput.dataSetup.changeSpeed = false;
									dataUserInput.dataSetup.indexArraySpeed = 0;
									dataUserInput.dataSetup.speed = atoi(dataUserInput.dataSetup.arraySpeed);
									// sscanf(dataUserInput.dataSetup.arraySpeed, "%d", &dataUserInput.dataSetup.speed);
									stateSaveSetup = true; // START SAVE SETUP
								}
							}
						}
						else if (dataUserInput.indexPageInSetup == 2)
						{
							if (dataUserInput.indexSlotInSetup == 1)
							{
								// SET UP NUM INDEX
								if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
								{
									dataUserInput.dataSetup.changeNumIndex = true;
									if (keyPressCurr == '*')
									{
										keyPressCurr = '.';
									}
									if (dataUserInput.dataSetup.indexArrayNumI > sizeof(dataUserInput.dataSetup.arrayNumIndex))
									{
										dataUserInput.dataSetup.indexArrayNumI = 0;
									}
									dataUserInput.dataSetup.arrayNumIndex[dataUserInput.dataSetup.indexArrayNumI] = (char)keyPressCurr;
									dataUserInput.dataSetup.indexArrayNumI++;
								}
								// COMFIRM NUM INDEX -> SAVE TO FLASH
								else if (keyPressCurr == '#')
								{
									dataUserInput.dataSetup.changeNumIndex = false;
									dataUserInput.dataSetup.indexArrayNumI = 0;
									dataUserInput.dataSetup.numIndex = atoi(dataUserInput.dataSetup.arrayNumIndex);
									// sscanf(dataUserInput.dataSetup.arrayNumIndex, "%d", &dataUserInput.dataSetup.numIndex);
									stateSaveSetup = true;
								}
							}
						}
					}
					// INPUT TOTAL PAGE
					else if (screenCurr == INPUT_TOTAL_PAGE_SCREEN)
					{
						changeButton = true;
						if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
						{

							dataUserInput.dataSetupRotation.changeCreatPage = true;
							if (keyPressCurr == '*')
							{
								keyPressCurr = '.';
							}
							if (dataUserInput.dataSetupRotation.indexStrTotalPage < sizeof(dataUserInput.dataSetupRotation.strTotalPage))
							{
								dataUserInput.dataSetupRotation.strTotalPage[dataUserInput.dataSetupRotation.indexStrTotalPage] = (char)keyPressCurr;
								dataUserInput.dataSetupRotation.indexStrTotalPage++;
							}
							else
							{
								dataUserInput.dataSetupRotation.indexStrTotalPage = 0;
							}
						}
						else if (keyPressCurr == '#')
						{
							dataUserInput.dataSetupRotation.indexStrTotalPage = 0;
							dataPageW.totalPage = atoi(dataUserInput.dataSetupRotation.strTotalPage);
							if (dataPageW.totalPage >= 5)
							{
								dataPageW.totalPage = 5;
							}
							dataUserInput.dataSetupRotation.changeCreatPage = false;
							dataUserInput.dataSetup.totalPage = dataPageW.totalPage;
							dataWriteFlash.totalPage = dataPageW.totalPage;
							stateSaveSetup = true;
						}
					}
					// INPUT ROTATION
					else if (screenCurr == HAND_ROTATION_SCREEN)
					{
						changeButton = true;

						if (keyPressCurr != '#' && keyPressCurr != 'A' && keyPressCurr != 'B' && keyPressCurr != 'C' && keyPressCurr != 'D' && keyPressCurr != 'x')
						{
							dataUserInput.dataSetupRotation.changeCreatPage = true;
							if (keyPressCurr == '*')
							{
								keyPressCurr = '.';
							}
							if (dataUserInput.dataSetupRotation.indexStrRotation < 2)
							{
								dataUserInput.dataSetupRotation.strRotation[dataUserInput.dataSetupRotation.indexStrRotation] = (char)keyPressCurr;
								dataUserInput.dataSetupRotation.indexStrRotation++;
							}
							else
							{
								dataUserInput.dataSetupRotation.indexStrRotation = 0;
							}
						}
						else if (keyPressCurr == '#')
						{
							dataPageW.indexArrRCur = (dataUserInput.dataSetupRotation.indexSlotInRotation) + ((dataUserInput.dataSetupRotation.indexPageInRotation - 1) * 3);
							dataPageW.dataRotation[dataPageW.page][dataPageW.indexArrRCur] = atoi(dataUserInput.dataSetupRotation.strRotation);
							if (dataPageW.dataRotation[dataPageW.page][dataPageW.indexArrRCur] >= dataUserInput.dataSetup.numIndex)
							{
								dataPageW.dataRotation[dataPageW.page][dataPageW.indexArrRCur] = dataUserInput.dataSetup.numIndex;
							}
							dataUserInput.dataSetupRotation.indexStrRotation = 0;
							memset(dataUserInput.dataSetupRotation.strRotation, '0', 2);
							dataUserInput.dataSetupRotation.changeCreatPage = false;
						}
					}
					else if (screenCurr == ANGLE_MAIN_SCREEN)
					{
						if (keyPressCurr == '#')
						{
							changeButton = true;
							// for (uint8_t i = 0; i < dataUserInput.dataAngleInMain.rotation_3; i++)
							// {
							// 	dataUserInput.RotationInTFT += ROUND / dataUserInput.IndexRotation;
							// }
							dataUserInput.RotationInTFT = dataUserInput.dataAngleInMain.rotation_3 * (ROUND / dataUserInput.IndexRotation);
						}
					}
				}
				{
					if (keyPressCurr == 'A') // ON/OFF MANUAL ANGLE
					{
						if (screenCurr != HAND_ROTATION_SCREEN && screenCurr != SETUP_SCREEN && screenCurr != INPUT_TOTAL_PAGE_SCREEN)
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
							screenCurr = INPUT_TOTAL_PAGE_SCREEN;
						}
						else if (screenCurr == INPUT_TOTAL_PAGE_SCREEN)
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
						if (screenCurr != HAND_ROTATION_SCREEN && screenCurr != SETUP_SCREEN && screenCurr != INPUT_TOTAL_PAGE_SCREEN)
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
				timeKeyPad = HAL_GetTick();
			}
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (isRun == false)
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
		else if (screenCurr == ANGLE_MAIN_SCREEN)
		{
			if (dataUserInput.dataAngleInMain.indexAngle < u8COLUMS)
			{
				dataUserInput.dataAngleInMain.indexAngle++;
			}
		}
		else if (screenCurr == FLOOR_MAIN_SCREEN)
		{
			dataUserInput.indexFloorInRotation++;
			dataUserInput.dataAngleInMain.indexAngle = 0;
			if (dataUserInput.indexFloorInRotation > dataUserInput.dataSetup.totalPage - 1)
			{
				dataUserInput.indexFloorInRotation = 1;
			}
		}
	}
	else if (GPIO_Pin == A2_Pin) // 	ENCODER RIGHT
	{
		changeEncoder2 = true;
		if (screenCurr == HAND_ROTATION_SCREEN)
		{
			dataUserInput.dataSetupRotation.changeCreatPage = false;
			if (strcmp(dataUserInput.dataSetupRotation.strRotation, "00") != 0)
			{
				memset(dataUserInput.dataSetupRotation.strRotation, '0', 2);
			}
			dataUserInput.dataSetupRotation.indexStrRotation = 0;
			if (dataUserInput.dataSetupRotation.indexPageInRotation < u8COLUMS / 3) // 90 : tong so vi tri co the luu
			{
				dataUserInput.dataSetupRotation.indexSlotInRotation++;
				if (dataUserInput.dataSetupRotation.indexSlotInRotation > 3)
				{
					dataUserInput.dataSetupRotation.indexSlotInRotation = 1;
					dataUserInput.dataSetupRotation.indexPageInRotation++;
				}
			}
			else if (dataUserInput.dataSetupRotation.indexPageInRotation >= u8COLUMS / 3)
			{
				dataUserInput.dataSetupRotation.indexPageInRotation = 1;
			}
		}
		else if (screenCurr == CHEATER_SCREEN)
		{
			dataUserInput.fCheater = dataUserInput.fCheater + _CHEATER;
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
		else if (screenCurr == ANGLE_MAIN_SCREEN)
		{
			if (dataUserInput.dataAngleInMain.indexAngle > 0)
			{
				dataUserInput.dataAngleInMain.indexAngle--;
			}
		}
		else if (screenCurr == FLOOR_MAIN_SCREEN)
		{
			dataUserInput.indexFloorInRotation--;
			dataUserInput.dataAngleInMain.indexAngle = 0;
			if (dataUserInput.indexFloorInRotation < 1)
			{
				dataUserInput.indexFloorInRotation = dataUserInput.dataSetup.totalPage;
			}
		}
	}
	else if (GPIO_Pin == A2_Pin) // 	ENCODER RIGHT
	{
		changeEncoder2 = true;
		if (screenCurr == HAND_ROTATION_SCREEN)
		{
			dataUserInput.dataSetupRotation.changeCreatPage = false;
			if (strcmp(dataUserInput.dataSetupRotation.strRotation, "00") != 0)
			{
				memset(dataUserInput.dataSetupRotation.strRotation, '0', 2);
			}
			dataUserInput.dataSetupRotation.indexStrRotation = 0;

			if (dataUserInput.dataSetupRotation.indexPageInRotation >= 1)
			{
				dataUserInput.dataSetupRotation.indexSlotInRotation--;
				if (dataUserInput.dataSetupRotation.indexSlotInRotation < 1)
				{
					dataUserInput.dataSetupRotation.indexSlotInRotation = 3;
					dataUserInput.dataSetupRotation.indexPageInRotation--;
				}
			}
			if (dataUserInput.dataSetupRotation.indexPageInRotation < 1)
			{
				dataUserInput.dataSetupRotation.indexPageInRotation = u8COLUMS / 3; // 45 /3
			}
		}
		else if (screenCurr == CHEATER_SCREEN)
		{
			dataUserInput.fCheater = dataUserInput.fCheater - _CHEATER;
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
	MX_TIM10_Init();
	MX_TIM9_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim9);
	// READ PULSE AND SPEED FROM FLASH
	{
		Flash_Read_Struct_Setting(AR_STORE_SETTING, &dataReadFlash, sizeof(dataSaveFlash_t));
		HAL_Delay(50);
		dataUserInput.dataSetup.speed = dataReadFlash.Speed;
		dataUserInput.dataSetup.pulse = atoi(dataReadFlash.Pulse);
		dataUserInput.dataSetup.numIndex = dataReadFlash.numIndex;

		dataUserInput.dataSetup.totalPage = dataReadFlash.totalPage;
		free(arrayRotationR);
		arrayRotationR = (uint8_t *)malloc((dataUserInput.dataSetup.totalPage * u8COLUMS) * sizeof(uint8_t));
		Flash_Read_Array(AR_STORE_ROTATION, arrayRotationR, dataUserInput.dataSetup.totalPage * u8COLUMS);
		// memcpy(dataTest, arrayRotationR, dataPageW.totalPage * u8COLUMS);
	}

	//  --------- END READ FLASH -----------------
	{
		dataUserInput.IndexRotation = dataUserInput.dataSetup.numIndex; //(32, 64, 96)
		dataUserInput.RotationInTFT = 0;								// 360

		dataUserInput.indexFloorInRotation = 1;
		dataUserInput.indexPageInSetup = 1;
		dataUserInput.indexSlotInSetup = 1;
		dataUserInput.fCheater = 0;
	}
	// DEFINE ANGLE NUMBER IN MAIN
	{
		dataUserInput.dataAngleInMain.rotation_1 = 0;
		dataUserInput.dataAngleInMain.rotation_2 = arrayRotationR[0];
		dataUserInput.dataAngleInMain.rotation_3 = arrayRotationR[1];
		dataUserInput.dataAngleInMain.rotation_4 = arrayRotationR[2];
		dataUserInput.dataAngleInMain.rotation_5 = arrayRotationR[3];
	}
	{
		dataUserInput.dataSetup.LoopMode = false;

		// DEFINE SPEED
		memset(dataUserInput.dataSetup.arraySpeed, '0', sizeof(dataUserInput.dataSetup.arraySpeed));
		dataUserInput.dataSetup.indexArraySpeed = 0;
		dataUserInput.dataSetup.changeSpeed = false;

		// DEFINE PULSE
		memset(dataUserInput.dataSetup.arrayPulse, '0', sizeof(dataUserInput.dataSetup.arrayPulse));
		dataUserInput.dataSetup.indexArrayPulse = 0;
		dataUserInput.dataSetup.changePulse = false;

		// DEFINE NUMINDEX
		memset(dataUserInput.dataSetup.arrayNumIndex, '0', sizeof(dataUserInput.dataSetup.arrayNumIndex));
		dataUserInput.dataSetup.indexArrayNumI = 0;
		dataUserInput.dataSetup.changeNumIndex = false;
	}
	// DEFINE TOTAL CREATE
	{
		dataUserInput.dataSetupRotation.indexPageInRotation = 1;
		dataUserInput.dataSetupRotation.indexSlotInRotation = 1;
	}

	// DEFINE DATA ROTATION WRITE TO FLASH
	{
		dataPageW.totalPage = 0;
		dataPageW.indexArrRCur = 1;
		dataPageW.page = 0;
	}
	// ------------ INIT ----------------
	initILI();	  // TFT
	initButton(); // BUTTON
	initKeypad(); // KEYPAD
	initStep();	  // TIMER , SET SPEED , PULSE ONE ROUND
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
	pulseStep = stepAngle(dataUserInput.RotationInTFT + dataUserInput.fCheater);
	ILI9341_Draw_Image((const char *)layoutMain, SCREEN_HORIZONTAL_2);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		buttonHandle();
		if (HAL_GetTick() - timeLed >= 100)
		{
			BlynkSlot(dataUserInput);
			if (isRun == true)
			{
				if (dataUserInput.dataSetup.LoopMode == true)
				{
					if (check == false)
					{
						check = true;
						ArrayTFT(screenCurr, dataUserInput);
					}
				}
			}
			else if (isRun == false)
			{
				if (stateSaveSetup == true) // SAVE SETTIN
				{
					stateSaveSetup = false;
					comfirmDataSetting();
				}
				if (stateSaveRotation == true) // SAVE ROTATION
				{
					stateSaveRotation = false;
					Flash_Write_Array(AR_STORE_ROTATION, arrayRotationW, dataPageW.totalPage * u8COLUMS);
					HAL_Delay(50);
					free(arrayRotationW);

					free(arrayRotationR);
					arrayRotationR = (uint8_t *)malloc((dataUserInput.dataSetup.totalPage * u8COLUMS) * sizeof(uint8_t));
					Flash_Read_Array(AR_STORE_ROTATION, arrayRotationR, dataUserInput.dataSetup.totalPage * u8COLUMS);

					// memcpy(dataTest, arrayRotationR, dataPageW.totalPage * u8COLUMS);
				}
				if (screenPrev != screenCurr) // CHANGE SCREEN
				{
					clearLCD();
					if (screenCurr == ENCODER_ROTATION_SCREEN || screenCurr == CHEATER_SCREEN || screenCurr == FLOOR_MAIN_SCREEN || screenCurr == ANGLE_MAIN_SCREEN)
					{
						ILI9341_Draw_Image((const char *)layoutMain, SCREEN_HORIZONTAL_2);
					}
					if (screenPrev == HAND_ROTATION_SCREEN && screenCurr != HAND_ROTATION_SCREEN)
					{
						for (int i = 0; i < dataPageW.totalPage; i++)
						{
							free(dataPageW.dataRotation[i]);
						}
						free(dataPageW.dataRotation);
					}
					ArrayTFT(screenCurr, dataUserInput);
					screenPrev = screenCurr;
				}
				if (changeEncoder1 == true || changeEncoder2 == true || changeButton == true) // HANDLE ENCODER BUTTON
				{
					changeEncoder1 = false;
					changeEncoder2 = false;
					changeButton = false;
					if (screenCurr == ENCODER_ROTATION_SCREEN || screenCurr == CHEATER_SCREEN)
					{
						pulseStep = stepAngle(dataUserInput.RotationInTFT + dataUserInput.fCheater);
					}

					else if (screenCurr == ANGLE_MAIN_SCREEN || screenCurr == FLOOR_MAIN_SCREEN || screenCurr == ENCODER_ROTATION_SCREEN)
					{
						uint8_t indexColums, indexRow;
						indexColums = dataUserInput.dataAngleInMain.indexAngle;
						indexRow = dataUserInput.indexFloorInRotation - 1;

						uint8_t num = indexRow * u8COLUMS + indexColums;

						if (indexColums == 0)
						{
							dataUserInput.dataAngleInMain.rotation_1 = 0;
							dataUserInput.dataAngleInMain.rotation_2 = arrayRotationR[num];
						}
						else if (indexColums > 0) // 1
						{
							dataUserInput.dataAngleInMain.rotation_1 = arrayRotationR[num - 1];
							dataUserInput.dataAngleInMain.rotation_2 = arrayRotationR[num];
						}

						dataUserInput.dataAngleInMain.rotation_3 = arrayRotationR[num + 1];
						dataUserInput.dataAngleInMain.rotation_4 = arrayRotationR[num + 2];
						dataUserInput.dataAngleInMain.rotation_5 = arrayRotationR[num + 3];

						pulseStep = stepAngle(dataUserInput.RotationInTFT + dataUserInput.fCheater);
					}

					ArrayTFT(screenCurr, dataUserInput);
				}

				// HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				timeLed = HAL_GetTick();
			}
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
