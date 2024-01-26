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
uint16_t inputAngle = 10;

float ROUND = 360;
#define  DOT  (ROUND / inputAngle)


volatile EncoderNew_t encoderCurr;
typedef enum 
{
		CW,
		CCW
}direction_t;

volatile direction_t dirEncoder1, dirEncoder2;
volatile int16_t valueEncoder1, valueEncoder2;

volatile bool changeData, changeEncoder1 ,changeEncoder2;


Screen_t screenCurr = AUTO_SCREEN, screenLast;

dataUser_t dataUserInput;

static uint32_t pulseStep2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t dataEncoder1 = 0, dataEncoder2 = 0;
uint64_t timeLed;
uint8_t stateBT1, stateBT2;
char key;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim10.Instance)
	{
			if(pulseStep2 != 0)
			{
					HAL_GPIO_TogglePin(PUL2_GPIO_Port, PUL2_Pin);
					pulseStep2--;
					HAL_TIM_Encoder_Stop_IT(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);
			}
			else 
			{
					HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);
			}
	}
	if(htim->Instance == htim11.Instance)
	{
			KEYPAD_Scan(0);
			key = getKey();
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance == htim2.Instance)
		{
			changeData = true;
			changeEncoder1 = true;
			encoderCurr = ENCODER1;
			
			dirEncoder1 = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
			valueEncoder1 = -__HAL_TIM_GetCounter(&htim2)/4;
			if(dirEncoder1 == CCW)		//  NGUOC
			{
					if(valueEncoder1 > 0)
					{
							dataEncoder1 = valueEncoder1;
					}
					else if (valueEncoder1 <= 0) 
					{
							dataEncoder1 = (ROUND / DOT) - abs(valueEncoder1);
							
					}
			}
			else if (dirEncoder1 == CW)		//  THUAN
			{
					if(valueEncoder1 > 0)
					{
							dataEncoder1 = valueEncoder1;
					}
					else if (valueEncoder1 <= 0)
					{
							dataEncoder1 = (ROUND / DOT) - abs(valueEncoder1);
					}
			}
			
			if(screenCurr == AUTO_SCREEN)
			{
					if(dataEncoder1 > (inputAngle - 1))
					{
							__HAL_TIM_SetCounter(&htim2, -4);
					}
					if(dataEncoder1 <= inputAngle || dataEncoder1 >= 0)								
					{
							dataUserInput.AngleStep = valueEncoder1 * (ROUND / inputAngle);
							dataUserInput.AngleTFT = valueEncoder1;
					}
			}
			else if(screenCurr == MANUAL_SCREEN)
			{
					if(dataEncoder1 <= 3 && dataEncoder1 > 0)
					{
							dataUserInput.page = dataEncoder1;
					}
					else if(dataEncoder1 > 3 || dataEncoder1 < 0)
					{	
							__HAL_TIM_SetCounter(&htim2, -4);
					}
			}
		}
		else if(htim->Instance == htim4.Instance)
		{
				valueEncoder2 = __HAL_TIM_GetCounter(&htim4) / 4;
				dirEncoder2 = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
				
				if(valueEncoder2 <= 360 && valueEncoder2 > 0)
				{
						if(dirEncoder2 == CCW)		//  NGUOC
						{
								if(valueEncoder2 > 0)
								{
										dataEncoder2 = valueEncoder2;
										
								}
								else if (valueEncoder2 <= 0) 
								{
										dataEncoder2 = (ROUND / DOT) - abs(valueEncoder2);
										
								}
						}
						else if (dirEncoder2 == CW)		//  THUAN
						{
								if(valueEncoder2 > 0)
								{
										dataEncoder2 = valueEncoder2;
								}
								else if (valueEncoder2 <= 0)
								{
										dataEncoder2 = (ROUND / DOT) - abs(valueEncoder2);
								}
						}
						dataUserInput.encoder2 = dataEncoder2 - 1;
				}
				else if (valueEncoder2 > 360 || valueEncoder2 <= 0)
				{
						__HAL_TIM_SetCounter(&htim4, 4);
				}
				changeData = true;
				changeEncoder2 = true;
				encoderCurr = ENCODER2;
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
  MX_TIM2_Init();
  MX_TIM9_Init();
  MX_TIM4_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_Base_Start_IT(&htim10);
	
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin,1);
	
	initButton();
	initILI();
	
	dataUserInput.numAngle = DOT;
	dataUserInput.OneRound = 1;
	dataUserInput.AngleStep = 0;
	changeEncoder1 = true;
	changeEncoder2 = false;
	drawCircleAngle(dataUserInput);
	
	HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
	__HAL_TIM_SetCounter(&htim2, -4);
	__HAL_TIM_SetCounter(&htim4, 4);
	initKeypad();
	
	//ILI9341_Draw_Image((const char*)snow_tiger, SCREEN_VERTICAL_2);
	//ILI9341_Draw_Image((const char*)logo, SCREEN_HORIZONTAL_2);
	//HAL_Delay(5000);
	clearLCD();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		stateBT1 = HAL_GPIO_ReadPin(BT1_GPIO_Port, BT1_Pin);
		stateBT2 = HAL_GPIO_ReadPin(BT2_GPIO_Port, BT2_Pin);
		buttonHandle();
		
		if(HAL_GetTick() - timeLed > 100)
		{
				if(screenLast != screenCurr)
				{
						clearLCD();
						ArrayTFT(screenCurr, dataUserInput);
						screenLast = screenCurr;
				}
				if(changeData == true)
				{
						changeData = false;
						ArrayTFT(screenCurr, dataUserInput);
						pulseStep2 = stepAngle(dataUserInput.AngleStep);
				}
				//HAL_GPIO_TogglePin(DIR2_GPIO_Port, DIR2_Pin);
				//pulse = 6400;
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				timeLed = HAL_GetTick();
		}
		dataUserInput.numAngle = DOT;
		
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
