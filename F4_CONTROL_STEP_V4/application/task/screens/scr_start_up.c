#include "ak.h"
#include "timer.h"

#include "app.h"
#include "task_list.h"

#include "scr_start_up.h"
#include "scr_device.h"

#include "view_manager.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

void view_scr_startup() {
#define AK_LOGO_AXIS_X		(80)
#define AK_LOGO_TEXT		(AK_LOGO_AXIS_X + 25)
	/* ak logo */
	screen_clear();
	
	ILI9341_Draw_Text("     ____        __   __	"	, AK_LOGO_AXIS_X, 20, WHITE, BLACK, Font1);
	ILI9341_Draw_Text("    /      \\      (   ) /  )"	, AK_LOGO_AXIS_X, 35, WHITE, BLACK, Font1);
	ILI9341_Draw_Text("   /  __   \\   	 (       /"	,AK_LOGO_AXIS_X, 50, WHITE, BLACK, Font1);
	ILI9341_Draw_Text("  /  (__)   \\  	 (      ("	, AK_LOGO_AXIS_X, 65, WHITE, BLACK, Font1);
	ILI9341_Draw_Text(" /   /  \\    \\   (       \\"	, AK_LOGO_AXIS_X, 80, WHITE, BLACK, Font1);
	ILI9341_Draw_Text("( __ )  ( __ )  (__)\\__)"	,AK_LOGO_AXIS_X, 95, WHITE, BLACK, Font1);
	ILI9341_Draw_Text("Active Kernel"	,AK_LOGO_TEXT, 130, WHITE, BLACK, Font1);
}

void task_screen(ak_msg_t *msg)
{
	switch(msg->sig)
	{
		case SCREEN_START_UP_SIG:
			screen_init();
			view_scr_startup();
			timer_set(TASK_SCREEN_DEVICE_ID, SCREEN_SETUP_SIG, AC_DISPLAY_STARTUP_INTERVAL, TIMER_ONE_SHOT);
			
			break;
		
	}
}
