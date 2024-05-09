#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__


#include "ak.h"
#include "task.h"

extern const task_t 		app_task_table[];
extern const task_polling_t app_task_polling_table[];

/*****************************************************************************/
/*  DECLARE: Internal Task ID
 *  Note: Task id MUST be increasing order.
 */
/*****************************************************************************/
enum
{
	/* SYSTEM TASKS */
	TASK_TIMER_TICK_ID,
	
	/* APP TASKS */
	AC_TASK_LIFE_ID,
	TASK_SCREEN_ID,
	TASK_SCREEN_DEVICE_ID,
	TASK_KEYPAD_ID,
	TASK_FLASH_ID,
	TASK_STEP_ID,
	/* EOT task ID */
	AK_TASK_EOT_ID
};

enum
{
	/* EOT polling task ID */
	AK_TASK_POLLING_EOT_ID
};

/*****************************************************************************/
/*  DECLARE: Task entry point
 */
/*****************************************************************************/
/* APP TASKS */
extern void task_system(ak_msg_t *);
extern void task_life(ak_msg_t *);

extern void task_screen(ak_msg_t *);
extern void task_screen_device_handle(ak_msg_t *);
extern void task_keypad_handle(ak_msg_t *);
extern void task_flash_handle(ak_msg_t *);
extern void task_step_handle(ak_msg_t *);
#endif