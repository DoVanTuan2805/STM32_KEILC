#include "plate.h"

#include "gpio.h"

bool cplt_plate_in = false;			// complete plate in
bool cplt_plate_out = false;			// complete plate out

bool ctht_plate_out;
bool ctht_plate_in;
void plate_stop()
{
	HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, 0);
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 0);
}
	
void plate_out()
{
	cplt_plate_out = false;
	ctht_plate_out = HAL_GPIO_ReadPin(CTHT_PLATE_OUT_GPIO_Port, CTHT_PLATE_OUT_Pin);
	while(HAL_GPIO_ReadPin(CTHT_PLATE_OUT_GPIO_Port, CTHT_PLATE_OUT_Pin) == 1)
	{
		ctht_plate_out = HAL_GPIO_ReadPin(CTHT_PLATE_OUT_GPIO_Port, CTHT_PLATE_OUT_Pin);
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, 1);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 0);
	}
	plate_stop();
	cplt_plate_out = true;
}

void plate_in()
{
	cplt_plate_in = false;
	ctht_plate_in = HAL_GPIO_ReadPin(CTHT_PLATE_IN_GPIO_Port, CTHT_PLATE_IN_Pin);
	while(HAL_GPIO_ReadPin(CTHT_PLATE_IN_GPIO_Port, CTHT_PLATE_IN_Pin) == 1)
	{
		ctht_plate_in = HAL_GPIO_ReadPin(CTHT_PLATE_IN_GPIO_Port, CTHT_PLATE_IN_Pin);
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, 0);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 1);
	}
	plate_stop();
	cplt_plate_in = true;
}
