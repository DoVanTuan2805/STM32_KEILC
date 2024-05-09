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
#define AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL (100)

	/* define signal */
enum
{
	AC_LIFE_SYSTEM_CHECK = AK_USER_DEFINE_SIG,
};


#ifdef __cplusplus
}
#endif


#endif