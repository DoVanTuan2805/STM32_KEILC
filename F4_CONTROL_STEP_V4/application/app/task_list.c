#include "task_list.h"
#include "timer.h"

const task_t app_task_table[] = {
	/*************************************************************************/
	/* SYSTEM TASK */
	/*************************************************************************/
	{TASK_TIMER_TICK_ID,		TASK_PRI_LEVEL_7,		task_timer_tick		},
	
	
	/*************************************************************************/
	/* APP TASK */
	/*************************************************************************/
	{AC_TASK_LIFE_ID			,TASK_PRI_LEVEL_6	,	task_life			},
	
	{TASK_SCREEN_ID				,TASK_PRI_LEVEL_4	,	task_screen			},
	{TASK_SCREEN_DEVICE_ID		,TASK_PRI_LEVEL_4	, 	task_screen_device_handle},
	{TASK_KEYPAD_ID				,TASK_PRI_LEVEL_4	, 	task_keypad_handle},
	{TASK_FLASH_ID				,TASK_PRI_LEVEL_4	, 	task_flash_handle},
	{TASK_STEP_ID				,TASK_PRI_LEVEL_4	, 	task_step_handle},
	{AK_TASK_EOT_ID				,TASK_PRI_LEVEL_0	,	(pf_task)0		}
};


const task_polling_t app_task_polling_table[] = {
	{AK_TASK_POLLING_EOT_ID		,	AK_DISABLE	,	(pf_task_polling)0			},
};
