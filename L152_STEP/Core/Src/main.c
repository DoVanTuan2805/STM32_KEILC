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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cell_car.h"
#include "drivenStep.h"
#include "math.h"
#include "pallet.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "stepFloor.h"
#include "stepPosition.h"
#include "string.h"
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
typedef enum {
    PROCESS_RFID,
    PROCESS_FIND_EMPTY_CAR,
    PROCESS_GO_EMPTY_CAR,
    PROCESS_PALLET,
    PROCESS_PALLET_GO_OUT,    // PALLET DI RA
    PROCESS_PALLET_GO_IN,     // PALLET DI VAO
    PROCESS_INCREASE_PALLET,  // TANG PALLET
    PROCESS_DECREASE_PALLET,  // GIAM PALLET
    PROCESS_GO_IN,
    PROCESS_GO_OUT,
    PROCESS_OFF
} process_step_t;
process_step_t process_step;

stepPosition_t stepP = {};
stepFloor_t stepF = {};
extern eFloor_t floorRead;
extern ePosition_t positionRead;
uint8_t viTri = 0, soTang = 0;

uint64_t angle_floor = 0, angle_position = 0;
volatile uint64_t pulseP = 0, pulseF = 0;
uint64_t pulseStepP = 0, pulseStepF = 0;
bool flag_complete = false;

extern uint8_t CardID_I[5], lastCardID_I[5];
extern uint8_t CardID_II[5], lastCardID_II[5];
extern bool flagRFID_In, flagRFID_Out;
	
uint64_t time_led;
uint64_t prev_time_RFID_In = 0, prev_time_RFID_Out = 0;
volatile uint64_t prev_time_goHome = 0;

#define SIZE_DATA 128
volatile uint8_t buffer_data[SIZE_DATA] = {};
volatile uint8_t buffer_data_ESP[SIZE_DATA] = {};
uint8_t RxData, RxDataESP;
volatile uint8_t i_STM = 0, i_ESP = 0;
bool flagRx = false;
char *dataRead;  // data parse

bool changeInOut = false;
bool tc_home, ctht_home;
bool complete_home = false;
float distance = 0;

extern bool arr_car_empty[NUM_CELL];
int led = 0;

volatile int16_t encoder_read = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == EXTI_IN_A_Pin) {
        if (HAL_GPIO_ReadPin(IN_B_GPIO_Port, IN_B_Pin)) {
            encoder_read--;
        } else {
            encoder_read++;
        }
    }
}



char *parseData(char *uartBuffer, uint8_t pos) {
    char *data;
    char *argv[10];
    uint8_t argvNum = 0;
    data = strtok((char *)uartBuffer, " ");

    while (data != NULL) {
        argv[argvNum] = data;
        data = strtok(NULL, " ");
        argvNum++;
    }
    if (pos > argvNum) {
        return "NULL";
    }
    return argv[pos];
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == huart2.Instance) {
        buffer_data[i_STM++] = RxData;
        if (RxData == '\n') {
            i_STM = 0;
            sscanf((char *)buffer_data, "%d %d\n", &viTri, &soTang);
            flagRx = true;
        }
        HAL_UART_Receive_IT(&huart2, &RxData, 1);
    } else if (huart->Instance == huart3.Instance) {
        buffer_data_ESP[i_ESP++] = RxDataESP;
        if (RxDataESP == '\n') {
            i_ESP = 0;
            dataRead = parseData((char *)buffer_data_ESP, 0);
            if (strcmp(dataRead, "SR") == 0) {
                dataRead = parseData((char *)buffer_data_ESP, 1);
                distance = atof(dataRead);
                // HAL_UART_Transmit(&huart2, (uint8_t *)"\nCOMPLETE",
                // sizeof("\nCOMPLETE"), 100);
            }
        }
        HAL_UART_Receive_IT(&huart3, &RxDataESP, 1);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == htim2.Instance)  // 80us
    {
        if (pulseP != 0) {
            flag_complete = false;
            pulseP--;
            HAL_GPIO_TogglePin(PUL2_GPIO_Port, PUL2_Pin);
        }
        if (pulseF != 0) {
            flag_complete = false;
            pulseF--;
            HAL_GPIO_TogglePin(PUL1_GPIO_Port, PUL1_Pin);
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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

    tc_home = HAL_GPIO_ReadPin(TC_HOME_GPIO_Port, TC_HOME_Pin);
    ctht_home = HAL_GPIO_ReadPin(CTHT_HOME_GPIO_Port, CTHT_HOME_Pin);

    // INIT UART
    {
        HAL_UART_Receive_IT(&huart2, &RxData, 1);
        HAL_UART_Receive_IT(&huart3, &RxDataESP, 1);
        HAL_UART_Transmit(&huart2, (uint8_t *)"START\n", 6, 100);
    }

    // STEP CONSTRUCTOR
    {
        step_floor_ctor(&stepF, PUL1_GPIO_Port, PUL1_Pin, DIR1_GPIO_Port,
                        DIR1_Pin);
        setPulse(&stepF.inforStep, 1600);

        step_position_ctor(&stepP, PUL2_GPIO_Port, PUL2_Pin, DIR2_GPIO_Port,
                           DIR2_Pin);
        setPulse(&stepP.inforStep, 80000);
    }

    InfrStep_t const *step[] = {&stepF.inforStep, &stepP.inforStep};

    // GET INFOR PULSE STEP
    {
        pulseStepP = getPulse(&stepP.inforStep);
        pulseStepF = getPulse(&stepF.inforStep);
    }

    HAL_TIM_Base_Stop_IT(&htim2);
    go_home();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
		
        tc_home = HAL_GPIO_ReadPin(TC_HOME_GPIO_Port, TC_HOME_Pin);
        ctht_home = HAL_GPIO_ReadPin(CTHT_HOME_GPIO_Port, CTHT_HOME_Pin);
        read_E18();

        if (flagRx == true) {
			flagRx = false;
            set_floor(&stepF, soTang);
            set_position(&stepP, viTri);
            HAL_TIM_Base_Start_IT(&htim2);
        }
        angle_position = get_angle_position(&stepP);
        angle_floor = get_angle_floor(&stepF);
        if (angle_position != 0) {
            pulseP = round((float)pulseStepP * ((float)angle_position / 360.0));
            reseet_angle_position(&stepP);
        }
        if (angle_floor != 0) {
            pulseF = round((float)pulseStepF * ((float)angle_floor / 360.0));
            reset_angle_floor(&stepF);
        }
        move_to(step, sizeof(step) / sizeof(step[0]));
		
		if(HAL_GetTick() - time_led >= 100)
		{
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			time_led = HAL_GetTick();
		}
        // HAL_Delay(10);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {
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
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
