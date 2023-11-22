/*
 * MOTOR_1KHZ.h
 *
 *  Created on: Jun 25, 2023
 *      Author: plm12
 */

#ifndef INC_MOTOR_1KHZ_H_
#define INC_MOTOR_1KHZ_H_

#include "main.h"

typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	GPIO_TypeDef *MOTOR_PORT;
	uint16_t *MOTOR_Pin;
} Motor_t;

typedef enum {
	CCW, CW
} Direct_t;

void MOTOR_Init(Motor_t *Motor, TIM_HandleTypeDef *htim, uint32_t Channel,
		GPIO_TypeDef *Port, uint16_t *Pin);
void SPEED(Motor_t *Motor, uint16_t MOTOR_SPEED, Direct_t DIR);
#endif /* INC_MOTOR_1KHZ_H_ */
