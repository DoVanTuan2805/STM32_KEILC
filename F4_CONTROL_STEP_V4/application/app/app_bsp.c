#include "ak.h"
#include "timer.h"

#include "app_bsp.h"

#include "app.h"
#include "task_list.h"

#include "scr_device.h"

button_t button_up;
button_t button_down;

#define CALCULATE_ANGLE()\
((angle_config[user_step_info.user_scr_main.floor][user_step_info.user_scr_main.ptr_tble_angle] * \
(CIRCLE_RAD / user_step_info.user_setting.index)))


void btn_down_callback(void* b) {
    button_t* me_b = (button_t*)b;
    switch (me_b->state) {
        case BUTTON_SW_STATE_PRESSED: {

        } break;

        case BUTTON_SW_STATE_LONG_PRESSED: {


        } break;

        case BUTTON_SW_STATE_RELEASED: {
            if(screen_info.screen_option == SCREEN_OPTION_RUN) {
                if (screen_info.screen_main_option == SCREEN_MAIN_ANGLE_HAND) {
                    user_step_info.user_scr_main.ptr_tble_angle -= 1;
                    user_step_info.user_scr_main.ptr_tble_angle = user_step_info.user_scr_main.ptr_tble_angle < 0
													?  (ANGLE_FLOOR_MAX - 1) 
													: user_step_info.user_scr_main.ptr_tble_angle;
                    user_step_info.user_scr_main.step_deg = CALCULATE_ANGLE();
                    task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                                        SCREEN_MAIN_CHANGE_OPTION_SIG);
                    timer_set(TASK_STEP_ID, STEP_UPDATE_DEG_SIG, 500, TIMER_ONE_SHOT);
                }
            }
        } break;

        default:
            break;
    }
}
void btn_up_callback(void* b) {
    button_t* me_b = (button_t*)b;
    switch (me_b->state) {
        case BUTTON_SW_STATE_PRESSED: {

        } break;

        case BUTTON_SW_STATE_LONG_PRESSED: {

        } break;

        case BUTTON_SW_STATE_RELEASED: {
            if(screen_info.screen_option == SCREEN_OPTION_RUN) {
                if (screen_info.screen_main_option == SCREEN_MAIN_ANGLE_HAND) {
                    user_step_info.user_scr_main.ptr_tble_angle += 1;
                    user_step_info.user_scr_main.ptr_tble_angle = user_step_info.user_scr_main.ptr_tble_angle > (ANGLE_FLOOR_MAX - 1)
                                                                ? 0
                                                                : user_step_info.user_scr_main.ptr_tble_angle;
                    user_step_info.user_scr_main.step_deg = CALCULATE_ANGLE();
                    task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                                    SCREEN_MAIN_CHANGE_OPTION_SIG);
                    timer_set(TASK_STEP_ID, STEP_UPDATE_DEG_SIG, 500, TIMER_ONE_SHOT);
                }
            }
            break;
        }   

        default:
            break;
    }
}