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
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "ds18b20.h"
#include "i2c-lcd.h"
#include "delay_timer.h"
#include "stdio.h"
#include "string.h"
#include "button.h"
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
Button_t btUp, btDown;
CLCD_I2C_Name lcd;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SAMPLING_TIME 200
#define MAX 128
#define MIN 0
float Temp;
typedef struct
{
	uint8_t setPoint;
	float kp;
	float ki;
	float kd;
	int16_t prevError;
	int16_t error;
	int16_t P;
	int16_t I;
	int16_t D;
}PID_t;
volatile PID_t pid;
volatile int16_t total;
char dataLine1[16], dataLine2[16];
volatile bool zero;
volatile uint8_t i = 0;
volatile uint8_t dim = 0;
uint64_t timeLed;
uint8_t check;
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
long constrain (long input, long minValue, long maxValue)
{
	if(input < minValue)
	{
		return minValue;
	}
	else if (input > maxValue)
	{
		return maxValue;
	}
	else 
	{
		return input;
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == ZERO_Pin)			/// 50hz = 0.02s
	{
		zero = true;
		i = 0;
		HAL_GPIO_WritePin(DIM_GPIO_Port, DIM_Pin, 0);
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == htim3.Instance)		// 75us	= 0.0013
	{
			if (i >= dim) {
				HAL_GPIO_WritePin(DIM_GPIO_Port, DIM_Pin, 1);
				i = 0;
				zero = false;
			} else {
				i++;
			}
	}
	else if(htim->Instance == htim2.Instance)			// 200ms
	{
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		
			pid.error = pid.setPoint -  Temp - 1 ;
			
			pid.P = pid.kp * pid.error;
			pid.I += pid.ki * pid.error;
			if(pid.I > MAX)
				pid.I = MAX;
			else if (pid.I < MIN)
			{
				pid.I = MIN;
			}
			pid.D = pid.kd * ((pid.error - pid.prevError) / SAMPLING_TIME);
			
			total = pid.P + pid.I + pid.D;
			pid.prevError = pid.error;
			total = constrain(total, MIN, MAX);
						// 
			if(Temp >= pid.setPoint + 1)
			{
				 dim = 128;
			} 
			else 
			{
				dim = map(total , 0, 128, 128, 10);
			}
		}
}
void bt_press_callback(Button_t *button) 
{
	if(button == &btDown)
	{
		pid.setPoint--;
	}
	if (button == &btUp)
	{
		pid.setPoint++;
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
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);			// 
	HAL_TIM_Base_Start_IT(&htim3);
	Button_Init(&btUp, UP_GPIO_Port, UP_Pin);
	Button_Init(&btDown, DOWN_GPIO_Port, DOWN_Pin);
	DS18B20_Init(DS18B20_Resolution_12bits);
	CLCD_I2C_Init(&lcd,&hi2c1,0x4e,16,2);	
	CLCD_I2C_Clear(&lcd);
	
	pid.kp = 11;
	pid.ki = 0.5;
	pid.kd = 0;
	pid.setPoint = 30;
//	DELAY_TIM_Init(&htim4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	// welcome
	{
		sprintf(dataLine1 , " HE THONG DIEU");
		CLCD_I2C_SetCursor(&lcd, 1,0);
		CLCD_I2C_WriteString(&lcd, dataLine1);
		sprintf(dataLine1 , "KHIEN NHIET DO");
		CLCD_I2C_SetCursor(&lcd, 1,2);
		CLCD_I2C_WriteString(&lcd, dataLine1);
		HAL_Delay(1500);
		CLCD_I2C_Clear(&lcd);
	}
  while (1)
  {
		bt_handle(&btUp);
		bt_handle(&btDown);
		//HAL_GPIO_WritePin(DIM_GPIO_Port, DIM_Pin, 0);
		
		
		if(HAL_GetTick() - timeLed > 100)
		{
				DS18B20_ReadAll();
				DS18B20_StartAll();
			
				for(int i = 0; i < DS18B20_Quantity(); i++)
				{
						DS18B20_GetTemperature(i, &Temp);
				}
				
				sprintf(dataLine1 , "PV : %2.2f", Temp);
				CLCD_I2C_SetCursor(&lcd, 2,0);
				CLCD_I2C_WriteString(&lcd, dataLine1);
				sprintf(dataLine2 , "SV : %d", pid.setPoint);
				CLCD_I2C_SetCursor(&lcd, 2,2);
				CLCD_I2C_WriteString(&lcd, dataLine2);
				memset(dataLine1, '\0', sizeof(dataLine1));
				memset(dataLine2, '\0', sizeof(dataLine2));
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
