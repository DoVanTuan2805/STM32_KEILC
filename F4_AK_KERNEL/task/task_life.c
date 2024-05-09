#include "fsm.h"
#include "port.h"
#include "message.h"

#include "app.h"

#include "task_list.h"
#include "task_life.h"

#include "gpio.h"



void task_life(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_LIFE_SYSTEM_CHECK:
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	
		/* reset watchdog */
		
		break;

	default:
		break;
	}
}
