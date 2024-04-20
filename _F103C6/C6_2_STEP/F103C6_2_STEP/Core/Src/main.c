/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "stepFloor.h"
#include "stepPosition.h"

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
stepPosition_t stepP = {};
stepFloor_t stepF = {};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SIZE_DATA 128
volatile uint8_t pData[SIZE_DATA]  = {};
uint8_t RxData;
volatile uint8_t i = 0;
extern eFloor_t floorRead;
extern ePosition_t positionRead;

uint8_t num1 = 0, num2 = 0;
bool flagRx = false;

int led = 0;
uint16_t angle_floor = 0, angle_position = 0;
volatile uint64_t pulseP = 0, pulseF = 0;
uint16_t pulseStepP = 0, pulseStepF = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
		pData[i++] = RxData;
		if(RxData == '\n')
		{
		  i = 0;
		  sscanf((char*)pData, "%d %d\n", &num1 , &num2);
		  flagRx = true;
		}
		HAL_UART_Receive_IT(&huart1,&RxData, 1);
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim2.Instance)
  {
    if(pulseP != 0)
    {
      pulseP--;
      HAL_GPIO_TogglePin(STEP_Z_GPIO_Port, STEP_Z_Pin);
    }
    if(pulseF != 0)
    {
      pulseF--;
      HAL_GPIO_TogglePin(STEP_Y_GPIO_Port, STEP_Y_Pin);
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_GPIO_WritePin(DIR_Z_GPIO_Port, DIR_Z_Pin, 1);
  HAL_GPIO_WritePin(DIR_Y_GPIO_Port, DIR_Y_Pin, 0);

  HAL_GPIO_WritePin(MS1_Z_GPIO_Port, MS1_Z_Pin, 1);   // config 1600
  HAL_GPIO_WritePin(MS2_Z_GPIO_Port, MS2_Z_Pin, 1);
  HAL_GPIO_WritePin(MS3_Z_GPIO_Port, MS3_Z_Pin, 1);

  HAL_GPIO_WritePin(MS1_Y_GPIO_Port, MS1_Y_Pin, 1);   // config 1600
  HAL_GPIO_WritePin(MS2_Y_GPIO_Port, MS2_Y_Pin, 1);
  HAL_GPIO_WritePin(MS3_Y_GPIO_Port, MS3_Y_Pin, 1);

  // value = 6400 / 4;  // 1600 = 10 m m

  step_floor_ctor(&stepF, 
                  STEP_Y_GPIO_Port, 
                  STEP_Y_Pin,
                  DIR_Y_GPIO_Port,
                  DIR_Y_Pin,
                  LED_GPIO_Port,
                  LED_Pin
  );
  step_position_ctor(&stepP, 
                  STEP_Z_GPIO_Port, 
                  STEP_Z_Pin,
                  DIR_Z_GPIO_Port,
                  DIR_Z_Pin,
                  LED_GPIO_Port,
                  LED_Pin
  );
  InfrStep_t const* step[] = 
  {
    &stepF.inforStep,
	  &stepP.inforStep
  };
  

  HAL_GPIO_TogglePin(DIR_Z_GPIO_Port, DIR_Z_Pin);
  HAL_GPIO_TogglePin(DIR_Y_GPIO_Port, DIR_Y_Pin);
  
  
  HAL_UART_Receive_IT(&huart1, &RxData, 1);

  pulseStepP = getPulse(&stepP.inforStep);
  pulseStepF = getPulse(&stepF.inforStep);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(flagRx == true)
    {
      set_floor(&stepF, num2);
      set_position(&stepP, num1);
      flagRx = false;
    }
    
	  angle_position = get_angle_position(&stepP);
    angle_floor = get_angle_floor(&stepF);
    if(angle_position != 0)
    {
      pulseP = (float)pulseStepP * ((float)angle_position / 360.0);
      reseet_angle_position(&stepP);
    }
    if(angle_floor != 0)
    {
      pulseF = (float)pulseStepF *((float)angle_floor / 360.0);
      reset_angle_floor(&stepF);
    }
    move_to(step, sizeof(step)/ sizeof(step[0]));
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, led);
    //HAL_Delay(500);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
