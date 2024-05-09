#include "system.h"

#include "tim.h"

#include "app_keypad.h"

#include "task_step.h"

void timer10_irq_enable()
{
	HAL_TIM_Base_Start_IT(&htim10);
}

void timer10_irq_disable()
{
	HAL_TIM_Base_Start_IT(&htim10);
}


void timer11_irq_enable()
{
	HAL_TIM_Base_Start_IT(&htim11);
}

void timer11_irq_disable()
{
	HAL_TIM_Base_Stop_IT(&htim11);
}

void timer11_irq_handle()
{
	keypad_timer_polling();
}

void timer10_irq_handle()
{
	step_run();
}