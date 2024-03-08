#include "step_user.h"
#include "gpio.h"
#include "math.h"
#include "stdlib.h"
#include "tim.h"
#include "user_ex.h"
extern dataUser_t dataUserInput;

inforStep_t inforStep;
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void initStep(void)
{
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
	HAL_TIM_Base_Start_IT(&htim10);

	// READ FLASH
	inforStep.speed = map(dataUserInput.dataSetup.speed, 0, 100, 250, 59);
	inforStep.pulseOneRoud = dataUserInput.dataSetup.pulse;
	setSpeedStep(inforStep.speed);
	setPulseOneRound(inforStep.pulseOneRoud);

	inforStep.nextAngle = 0,
	inforStep.currentAngle = 0;
}
void setPulseOneRound(uint32_t u32Pulse)
{
	inforStep.pulseOneRoud = u32Pulse;
}
void setSpeedStep(uint32_t u32Speed)
{
	inforStep.speed = u32Speed;

	htim10.Instance = TIM10;
	htim10.Init.Prescaler = 84 - 1;
	htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim10.Init.Period = inforStep.speed - 1;
	htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	{
		Error_Handler();
	}
}

uint32_t stepAngle(float angle) // 360
{
	// 10 - 20
	float angleNew;
	uint32_t pulse;
	if (angle != inforStep.currentAngle)
	{
		if (inforStep.currentAngle >= 360)
		{
			angleNew = angle + 360;
			inforStep.currentAngle = angle;
		}
		else
		{
			inforStep.nextAngle = angle;
			angleNew = inforStep.nextAngle - inforStep.currentAngle;
			inforStep.currentAngle = inforStep.nextAngle;
		}

		uint8_t dirStep = angleNew < 0 ? 1 : 0;
		HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, dirStep);
		pulse = inforStep.pulseOneRoud * (fabs(angleNew) / 360.0);
	}
	else
	{
		pulse = 0;
	}
	return pulse;
}

/*
uint32_t stepAngle(float angle)
{
	float angleNew;
	uint32_t pulse;
	uint8_t dirStep;
	inforStep.nextAngle = angle;
	if (inforStep.nextAngle != inforStep.currentAngle)
	{
		float dimension = fabs(inforStep.nextAngle - inforStep.currentAngle);
		if (dimension < MID)
		{
			angleNew = dimension;
			dirStep = 0;
		}
		else if (dimension >= MID)
		{
			angleNew = (END - dimension);
			if (inforStep.nextAngle - inforStep.currentAngle < 0)
			{
				dirStep = 0;
			}
			else
			{
				dirStep = 1;
			}
		}
		HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, dirStep);
		pulse = inforStep.pulseOneRoud * (fabs(angleNew) / 360.0);

		inforStep.currentAngle = inforStep.nextAngle;
	}
	else
	{
		pulse = 0;
	}
	return pulse;
}
*/