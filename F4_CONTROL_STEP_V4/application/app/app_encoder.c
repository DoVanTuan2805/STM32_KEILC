#include "ak.h"
#include "timer.h"
#include "app_encoder.h"

#include "app.h"
#include "task_list.h"

#include "task_keypad.h"
#include "scr_device.h"

#include "encoder.h"


encoder_t encoder_left;
encoder_t encoder_right;

static inline void scr_encoder_mode(uint8_t dir) {
    if (dir == ENCODER_INCREASE) {
        user_step_info.user_scr_main.step_deg += CIRCLE_RAD / user_step_info.user_setting.index;
        if (user_step_info.user_scr_main.step_deg >= CIRCLE_RAD) {
            user_step_info.user_scr_main.step_deg = 0;
        }
    }
	else if (dir == ENCODER_DECREASE) {
        user_step_info.user_scr_main.step_deg -= CIRCLE_RAD / user_step_info.user_setting.index;
        if (user_step_info.user_scr_main.step_deg < 0) {
            user_step_info.user_scr_main.step_deg =
                CIRCLE_RAD - CIRCLE_RAD / user_step_info.user_setting.index;
        }
    }
    task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_MAIN_CHANGE_OPTION_SIG);
    timer_set(TASK_STEP_ID, STEP_UPDATE_DEG_SIG, 500, TIMER_ONE_SHOT);
}

static inline void scr_cheater_mode(uint8_t dir)
{
	if (dir == ENCODER_INCREASE) {
        user_step_info.user_scr_main.cheater += CHEATER_STEP;
    }
	else if (dir == ENCODER_DECREASE) {
         user_step_info.user_scr_main.cheater -= CHEATER_STEP;
    }
	task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_MAIN_CHANGE_OPTION_SIG);
    timer_set(TASK_STEP_ID, STEP_UPDATE_DEG_SIG, 500, TIMER_ONE_SHOT);
}

static inline void scr_floor_mode(uint8_t dir)
{
	if (dir == ENCODER_INCREASE) {
        user_step_info.user_scr_main.floor += 1;
		user_step_info.user_scr_main.floor = user_step_info.user_scr_main.floor > (FLOOR_MAX - 1) 
											?  0 
											: user_step_info.user_scr_main.floor;
        task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                           SCREEN_MAIN_CHANGE_OPTION_SIG);
    }
	else if (dir == ENCODER_DECREASE) {
		user_step_info.user_scr_main.floor -= 1;
        user_step_info.user_scr_main.floor = user_step_info.user_scr_main.floor < 0
											?  (FLOOR_MAX - 1) 
											: user_step_info.user_scr_main.floor;
        task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                           SCREEN_MAIN_CHANGE_OPTION_SIG);
    }
}

static inline void scr_tbl_angle_mode(uint8_t dir)
{
	if (dir == ENCODER_INCREASE) {
        user_step_info.user_scr_main.ptr_tble_angle += 1;
		user_step_info.user_scr_main.ptr_tble_angle = user_step_info.user_scr_main.ptr_tble_angle > (ANGLE_FLOOR_MAX - 1) 
													?  0 
													: user_step_info.user_scr_main.ptr_tble_angle;
        task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                           SCREEN_MAIN_CHANGE_OPTION_SIG);
    }
	else if (dir == ENCODER_DECREASE) {
		user_step_info.user_scr_main.ptr_tble_angle -= 1;
        user_step_info.user_scr_main.ptr_tble_angle = user_step_info.user_scr_main.ptr_tble_angle < 0
													?  (ANGLE_FLOOR_MAX - 1) 
													: user_step_info.user_scr_main.ptr_tble_angle;
        task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                           SCREEN_MAIN_CHANGE_OPTION_SIG);
    }
}

void encoder_left_callback(void* e)
{
	encoder_t* my_encoder = (encoder_t*) e;
	switch(my_encoder->state)
	{
		case ENCODER_INCREASE :
		{
			if(screen_info.screen_option == SCREEN_OPTION_CONFIG_ANGLE)
			{
				data_clear();
				user_step_info.user_config_angle.slot++;
				if(user_step_info.user_config_angle.slot > ANGLE_PAGE_MAX)
				{
					user_step_info.user_config_angle.slot = 0;
					user_step_info.user_config_angle.page++;
					if(user_step_info.user_config_angle.page > ANGLE_PAGE_MAX - 1)
					{
						user_step_info.user_config_angle.page = 0;
					}
				}
				task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                           SCREEN_CONFIG_ANGLE_CHANGE_SIG);
			}
			break;
		}
		case ENCODER_DECREASE:
		{
			if(screen_info.screen_option == SCREEN_OPTION_CONFIG_ANGLE)
			{
				data_clear();
				user_step_info.user_config_angle.slot--;
				if(user_step_info.user_config_angle.slot < 0)
				{
					user_step_info.user_config_angle.slot = ANGLE_PAGE_MAX;
					user_step_info.user_config_angle.page--;
					if(user_step_info.user_config_angle.page < 0)
					{
						user_step_info.user_config_angle.page = ANGLE_PAGE_MAX - 1;
					}
				}
				task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                           SCREEN_CONFIG_ANGLE_CHANGE_SIG);
			}
			break;
		}
	}
}

void encoder_right_callback(void* e)
{
	encoder_t* my_encoder = (encoder_t*) e;

	if(screen_info.screen_option == SCREEN_OPTION_RUN) {
		if(screen_info.screen_main_option == SCREEN_MAIN_ENCODER_HAND)
		{
			scr_encoder_mode(my_encoder->state);
		} 
		else if (screen_info.screen_main_option == SCREEN_MAIN_CHEATER_HAND)
		{
			scr_cheater_mode(my_encoder->state);
		} 
		else if (screen_info.screen_main_option == SCREEN_MAIN_FLOOR_HAND)
		{
			scr_floor_mode(my_encoder->state);
		}
		else if (screen_info.screen_main_option == SCREEN_MAIN_ANGLE_HAND)
		{
			scr_tbl_angle_mode(my_encoder->state);
		}	
	}
}