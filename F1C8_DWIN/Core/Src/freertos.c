/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "A7860C_command.h"
#include "MQTT_Sim_Config.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
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
/* USER CODE BEGIN Variables */
static MQTT_Infor_t mqttInfor;
static MQTT_Sub_t mqttSub;
extern uint8_t dataRx;
extern char bufferRx[128];
uint8_t checkCommand;	


/* USER CODE END Variables */
osThreadId blynkLedHandle;
osThreadId myTaskSimHandle;
osThreadId myTaskPingMqttHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void TaskBlynk(void const * argument);
void TaskSim(void const * argument);
void pingMqttTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of blynkLed */
  osThreadDef(blynkLed, TaskBlynk, osPriorityLow, 0, 128);
  blynkLedHandle = osThreadCreate(osThread(blynkLed), NULL);

  /* definition and creation of myTaskSim */
  osThreadDef(myTaskSim, TaskSim, osPriorityNormal, 0, 128);
  myTaskSimHandle = osThreadCreate(osThread(myTaskSim), NULL);

  /* definition and creation of myTaskPingMqtt */
  osThreadDef(myTaskPingMqtt, pingMqttTask, osPriorityIdle, 0, 128);
  myTaskPingMqttHandle = osThreadCreate(osThread(myTaskPingMqtt), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_TaskBlynk */
/**
  * @brief  Function implementing the blynkLed thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_TaskBlynk */
void TaskBlynk(void const * argument)
{
  /* USER CODE BEGIN TaskBlynk */
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    osDelay(100);
  }
  /* USER CODE END TaskBlynk */
}

/* USER CODE BEGIN Header_TaskSim */
/**
* @brief Function implementing the myTaskSim thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TaskSim */
void TaskSim(void const * argument)
{
  /* USER CODE BEGIN TaskSim */	
	uint16_t timePingMqtt;
	
	mqttInfor.clientID = "e2943989528d43d1";
	mqttInfor.clientIndex = 0;
	mqttInfor.sever = "test.mosquitto.org";
	mqttInfor.port = 1883;
	mqttInfor.keepAlive = 60;
	mqttInfor.cleanSession = 1;

	mqttSub.clientIndex = 0;
	mqttSub.Qos = 1;
	mqttSub.topic = "hello";
	HAL_UART_Receive_DMA(&huart1, &dataRx, 1);
	if(mountMQTT(mqttInfor) == 0)
	{
		osDelay(1000);
		subcribeTopic(mqttSub);
	}
	osDelay(100);
	vTaskResume(myTaskPingMqttHandle);
	
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
		timePingMqtt++;
		handleDataSub();
		if(timePingMqtt > 150)
		{
				timePingMqtt = 0;
				vTaskResume(myTaskPingMqttHandle);
				vTaskSuspend(NULL);
		}
		
  }
  /* USER CODE END TaskSim */
}

/* USER CODE BEGIN Header_pingMqttTask */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pingMqttTask */
uint8_t checkData;
void pingMqttTask(void const * argument)
{
  /* USER CODE BEGIN pingMqttTask */
	
	vTaskSuspend(NULL);
  /* Infinite loop */
  for(;;)
  {
		checkData = pingMQTT(mqttInfor);
		osDelay(100);
		vTaskResume(myTaskSimHandle);
		vTaskSuspend(NULL);
  }
  /* USER CODE END pingMqttTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
			uartReceive();
	}
}
/* USER CODE END Application */

