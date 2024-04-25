#include "task.h"

#include "main.h"

#include "lcd_user.h"
#include "config.h"
void task_screen()
{
	static uint64_t time_reload_screen;
	if( HAL_GetTick() - time_reload_screen >= 100)
	{
		ArrayTFT(screen_state, user_data);
	}
}

void task_encoder()
{
	
}