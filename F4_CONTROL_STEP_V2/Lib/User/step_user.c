#include "step_user.h"
#include "gpio.h"
#include "math.h"
#include "stdlib.h"

inforStep_t inforStep;

uint32_t stepAngle(float angle)
{
	inforStep.nextAngle = angle;
	
	uint32_t pulseStep2;
	float angleNew = inforStep.nextAngle - inforStep.currentAngle;
	uint8_t dirStep = angleNew < 0 ? 1 : 0;
	HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, dirStep);
	
	
	pulseStep2 = (3200 * fabsf(angleNew)) / 360;
	
	inforStep.currentAngle = inforStep.nextAngle;
	return pulseStep2;
}