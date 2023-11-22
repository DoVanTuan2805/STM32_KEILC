/*
 * MOTOR_1KHZ.c
 *
 *  Created on: Jun 25, 2023
 *      Author: plm12
 */
#include "MOTOR_1KHZ.h"


void PWM_MOTOR(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t SPEED);

void MOTOR_Init(Motor_t *Motor, TIM_HandleTypeDef *htim, uint32_t Channel,
		GPIO_TypeDef *Port, uint16_t *Pin) {
	Motor->htim = htim;
	Motor->Channel = Channel;
	Motor->MOTOR_PORT = Port;
	Motor->MOTOR_Pin = Pin;
	HAL_TIM_PWM_Start(Motor->htim, Motor->Channel);	// KHOI TAO PWM
}

void SPEED(Motor_t *Motor, uint16_t MOTOR_SPEED, Direct_t DIR) {

	switch (DIR) {
	case CCW:
		PWM_MOTOR(Motor->htim, Motor->Channel, MOTOR_SPEED);
		HAL_GPIO_WritePin(Motor->MOTOR_PORT, *Motor->MOTOR_Pin, 0);//0
		break;
	case CW:
		PWM_MOTOR(Motor->htim, Motor->Channel, 100- MOTOR_SPEED);
		HAL_GPIO_WritePin(Motor->MOTOR_PORT, *Motor->MOTOR_Pin, 1);
		break;
	default:
		break;
	}

}
void PWM_MOTOR(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t SPEED) {
	uint16_t ccr = SPEED * (htim->Instance->ARR / 100);
	switch (Channel) {
	case TIM_CHANNEL_1:
		htim->Instance->CCR1 = ccr;
		break;
	case TIM_CHANNEL_2:
		htim->Instance->CCR2 = ccr;
		break;
	case TIM_CHANNEL_3:
		htim->Instance->CCR3 = ccr;
		break;
	case TIM_CHANNEL_4:
		htim->Instance->CCR4 = ccr;
		break;
	default:
		break;
	}
}
