#include "plate.h"
#include "gpio.h"

void plate_stop()
{
	HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, 0);
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 0);
}
	
void plate_out()
{
	while(HAL_GPIO_ReadPin(CTHT_PLATE_OUT_GPIO_Port, CTHT_PLATE_OUT_Pin) == 1)
	{
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, 1);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 0);
	}
	plate_stop();
}

void plate_in()
{
	while(HAL_GPIO_ReadPin(CTHT_PLATE_IN_GPIO_Port, CTHT_PLATE_IN_Pin) == 1)
	{
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, 0);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 1);
	}
	plate_stop();
}
