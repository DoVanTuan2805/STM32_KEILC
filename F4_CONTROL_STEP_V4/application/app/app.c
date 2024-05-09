#include "app.h"

#include "ak.h"
#include "fsm.h"
#include "message.h"
#include "timer.h"

#include "ring_buffer.h"

#include "app.h"
#include "task_list.h"
#include "app_encoder.h"
#include "app_keypad.h"
#include "app_bsp.h"

#include "io_cfg.h"
#include "system.h"

#include "tim.h"

#include "button.h"
#include "encoder.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim11.Instance)
	{
		timer11_irq_handle();
	}
	else if (htim->Instance == htim10.Instance)
	{
		timer10_irq_handle();
	}
}

volatile uint16_t counter;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == A1_Pin) {
        encoder_irq(&encoder_left, HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin));
    } else if (GPIO_Pin == A2_Pin) {
        encoder_irq(&encoder_right, HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin));
    }
}

inline void sys_irq_timer_10ms()			// handle systick 50ms
{	
	button_timer_polling(&button_down);
	button_timer_polling(&button_up);
}

void app_start_timer_life()
{
	task_post_pure_msg(TASK_FLASH_ID, FLASH_SETUP_SIG);
	task_post_pure_msg(TASK_FLASH_ID, FLASH_READ_CFG_ANGLE_SIG);
	task_post_pure_msg(TASK_FLASH_ID, FLASH_READ_SETTING_SIG);

	task_post_pure_msg(TASK_STEP_ID, STEP_INIT_SIG);

	task_post_pure_msg(TASK_SCREEN_ID, SCREEN_START_UP_SIG);

	timer_set(AC_TASK_LIFE_ID, AC_LIFE_SYSTEM_CHECK, AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL, TIMER_PERIODIC);
	//timer_set(TASK_SCREEN_ID, SCREEN_START_UP_SIG, AC_DISPLAY_INITIAL_INTERVAL, TIMER_ONE_SHOT);
}

void app_init()
{
	ENTRY_CRITICAL();
    task_init();
    task_create((task_t*)app_task_table);
    task_polling_create((task_polling_t*)app_task_polling_table);
    EXIT_CRITICAL();
	
	app_start_timer_life();
	
	/* HW INIT	*/
	encoder_init(&encoder_left, 10, encoder_left_callback);
	encoder_init(&encoder_right, 10, encoder_right_callback);
	
	encoder_enable(&encoder_left);
	encoder_enable(&encoder_right);
	
	button_init(&button_down,	10,	1,	(void*)0,	button_down_read,	btn_down_callback);
	button_init(&button_up,	10,	1,	(void*)0,	button_up_read,	btn_up_callback);

	button_enable(&button_down);
	button_enable(&button_up);

	keypad_init();
	
	/*	TIMER IRQ INIT	*/
	timer11_irq_enable();
}
void app_run()
{
	task_run();
}

