#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "ak.h"

/*****************************************************************************/
/*  LIFE task define
 */
/*****************************************************************************/
/* define timer */
#define AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL (1000)

/* define signal */
enum
{
	AC_LIFE_SYSTEM_CHECK = AK_USER_DEFINE_SIG,
};

/*****************************************************************************/
/*  SCREEN task define
 */
/*****************************************************************************/
/* define timer */
#define AC_DISPLAY_INITIAL_INTERVAL  (1000)
#define AC_DISPLAY_STARTUP_INTERVAL  (2000)
#define AC_DISPLAY_ENCODER_HAND_TIMER_INTERVAL (500)

/* define signal */
enum
{
	SCREEN_START_UP_SIG = AK_USER_DEFINE_SIG,
	SCREEN_CHANGE,
	SCREEN_SETUP_SIG,
	SCREEN_RUN_SIG,
	SCREEN_MAIN_CHANGE_OPTION_SIG, 
	SCREEN_SETTING_CHANGE_OPTION_SIG,
	SCREEN_CONFIG_ANGLE_CHANGE_SIG,
};

/*****************************************************************************/
/*  FLASH task define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
enum
{
	FLASH_WRITE_CFG_ANGLE_SIG = AK_USER_DEFINE_SIG,
	FLASH_READ_CFG_ANGLE_SIG,
	FLASH_WRITE_SETTING_SIG,
	FLASH_READ_SETTING_SIG,
	FLASH_SETUP_SIG,
};

/*****************************************************************************/
/*  STEP task define
 */
/*****************************************************************************/
/* define timer */
#define AC_STEP_LOOP_MODE_INTERVAL	(50)
/* define signal */
enum 
{
	STEP_INIT_SIG = AK_USER_DEFINE_SIG,
	STEP_UPDATE_DEG_SIG,
	STEP_LOOP_MODE,
	STEP_DISABLE_SIG,
	STEP_ENABLE_SIG,
};

/*****************************************************************************/
/*  KEYPAD task define
 */
/*****************************************************************************/
/* define timer */
#define AC_KEYPAD_TIMER_INTERVAL	(100)
/* define signal */
enum
{
	KEYPAD_UPDATE_KEY_SIG = AK_USER_DEFINE_SIG,
};


/*****************************************************************************/
/*  BUTTON task define
 */
/*****************************************************************************/

extern void sys_irq_timer_10ms();	
extern void app_init();
extern void app_run();
#ifdef __cplusplus
}
#endif


#endif