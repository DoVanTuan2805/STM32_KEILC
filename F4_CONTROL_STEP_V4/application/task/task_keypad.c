#include "ak.h"
#include "timer.h"

#include "stdio.h"
#include "math.h"

#include "app.h"
#include "task_list.h"
#include "app_keypad.h"

#include "task_flash.h"
#include "task_keypad.h"
#include "scr_device.h"

#include "view_manager.h"

#define CALCULATE_ANGLE()   \
((angle_config[user_step_info.user_scr_main.floor][user_step_info.user_scr_main.ptr_tble_angle] * \
(CIRCLE_RAD / user_step_info.user_setting.index)))

char keypad_read, last_keypad_read;
char data[DATA_MAX];
uint8_t data_index;

void data_clear() {
    memset(data, '\0', sizeof(data));
    data_index = 0;
}

void config_angle_reset_parameter()
{
    user_step_info.user_config_angle.slot = 0;
    user_step_info.user_config_angle.page = 0;
    user_step_info.user_config_angle.floor = (FLOOR_MAX - 1);  ///  reset floor config 
}

static inline void keypad_input_config_angle(char key) {
#define FLOOR       (user_step_info.user_config_angle.floor)
#define SLOT        ( (user_step_info.user_config_angle.page * 5)  + user_step_info.user_config_angle.slot)

    if (key != '*' && key != 'x') {
			data[data_index++] = key;
        angle_config[FLOOR][SLOT] = atoi(data);
        angle_config[FLOOR][SLOT] = angle_config[FLOOR][SLOT] > user_step_info.user_setting.index \
                                    ? user_step_info.user_setting.index \
                                    : angle_config[FLOOR][SLOT];
        task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                                   SCREEN_CONFIG_ANGLE_CHANGE_SIG);
    }
}

static inline void keypad_input_setting(char key) {
    if (key == '#') {
        if (screen_info.screen_setting_option == SCREEN_SETTING_LOOPMODE) {
            user_step_info.user_setting.loopmode ^= 1;
            task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                               SCREEN_SETTING_CHANGE_OPTION_SIG);
            if (user_step_info.user_setting.loopmode == true) {
                timer_set(TASK_STEP_ID, STEP_LOOP_MODE, AC_STEP_LOOP_MODE_INTERVAL, TIMER_PERIODIC);
            } 
            else 
            {
                timer_remove_attr(TASK_STEP_ID, STEP_LOOP_MODE);
            }
        }
    } else if (key != '*' && key != 'x') {
        switch (screen_info.screen_setting_option) {
            case SCREEN_SETTING_PULSE: {
                data[data_index++] = key;
                user_step_info.user_setting.pulse = atoi(data);
                task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                                   SCREEN_SETTING_CHANGE_OPTION_SIG);
                break;
            }

            case SCREEN_SETTING_SPEED: {
                data[data_index++] = key;
                user_step_info.user_setting.speed = atoi(data);
                user_step_info.user_setting.speed =
                    user_step_info.user_setting.speed > SPEED_MAX  ? SPEED_MAX : user_step_info.user_setting.speed;
                task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                                   SCREEN_SETTING_CHANGE_OPTION_SIG);
                break;
            }

            case SCREEN_SETTING_INDEX: {
                data[data_index++] = key;
                user_step_info.user_setting.index = atoi(data);
                user_step_info.user_setting.index =  user_step_info.user_setting.index > INDEX_MAX ? INDEX_MAX : user_step_info.user_setting.index;
                task_post_pure_msg(TASK_SCREEN_DEVICE_ID,
                                   SCREEN_SETTING_CHANGE_OPTION_SIG);
                break;
            }

            default:
                break;
        }
    }
}

static inline void keypad_input_encoder_main(char key)
{
    if(key == '#')
    {
        if(data[0] == '\0')
        {
            return;
        }
        uint16_t _deg = atoi(data) * (CIRCLE_RAD / user_step_info.user_setting.index) ;
        if(_deg > CIRCLE_RAD)
        {
            _deg = CIRCLE_RAD;
        }
        user_step_info.user_scr_main.step_deg = _deg;
        data_clear();
        timer_set(TASK_STEP_ID, STEP_UPDATE_DEG_SIG, 500, TIMER_ONE_SHOT);
    }
    else if (key != '*' && key != 'x')
    {
        data[data_index++] = key;
    }
    task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_SETTING_CHANGE_OPTION_SIG);
}

static inline void keypad_control_main(char key) {
    screen_info.screen_setting_option = SCREEN_SETTING_NULL;  // reset screen setting

    screen_info.screen_option = SCREEN_OPTION_RUN;

    screen_info.screen_main_option++;
    if (screen_info.screen_main_option >= SCREEN_MAIN_NULL) {
        screen_info.screen_main_option = SCREEN_MAIN_ENCODER_HAND;
    }

    view_clear_change_option();
    task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_MAIN_CHANGE_OPTION_SIG);
    config_angle_reset_parameter();     // reset config angle
}

static inline void keypad_control_setting(char key) {
    screen_info.screen_main_option = SCREEN_MAIN_NULL;  // reset screen main

    screen_info.screen_option = SCREEN_OPTION_SETTING;

    screen_info.screen_setting_option++;
    if (screen_info.screen_setting_option >= SCREEN_SETTING_NULL) {
        screen_info.screen_setting_option = SCREEN_SETTING_LOOPMODE;
    }
    view_clear_change_option();
    task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_SETTING_CHANGE_OPTION_SIG);
    config_angle_reset_parameter();     // reset config angle
}

static inline void keypad_control_config_angle(char key) {
    screen_info.screen_setting_option = SCREEN_SETTING_NULL;  // reset screen setting
    screen_info.screen_main_option = SCREEN_MAIN_NULL;  // reset screen main

    screen_info.screen_option = SCREEN_OPTION_CONFIG_ANGLE;

    user_step_info.user_config_angle.floor++;
    if (user_step_info.user_config_angle.floor > (FLOOR_MAX - 1)) {
        user_step_info.user_config_angle.floor = 0;
    }
    view_clear_change_option();
    task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_CONFIG_ANGLE_CHANGE_SIG);
}

static inline void keypad_control_handle(char key) {
    // CHANGE SCREEN MAIN OPTION
    if (key == 'A') {
        data_clear();
        keypad_control_main(key);
    }
    // CHANGE SCREEN SETTING OPTION
    else if (key == 'D') {
        data_clear();
        keypad_control_setting(key);
    } 
    else if (key == 'B')  // SWITCH CONFIG ANGLE
    {
        data_clear();
        keypad_control_config_angle(key);
    } 
    else if (key != 'C') {
        if (screen_info.screen_option == SCREEN_OPTION_SETTING) {
            keypad_input_setting(key);
        }
        else if (screen_info.screen_option == SCREEN_OPTION_CONFIG_ANGLE)
        {
            keypad_input_config_angle(key);
        }
        else if (screen_info.screen_option == SCREEN_OPTION_RUN)
        {
            if(screen_info.screen_main_option == SCREEN_MAIN_ENCODER_HAND)
            {
                keypad_input_encoder_main(key);
            }
            else if(screen_info.screen_main_option == SCREEN_MAIN_ANGLE_HAND)
            {
                if(key == '#')
                {
                    user_step_info.user_scr_main.step_deg =  CALCULATE_ANGLE();
                    task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_MAIN_CHANGE_OPTION_SIG);
                    timer_set(TASK_STEP_ID, STEP_UPDATE_DEG_SIG, 500, TIMER_ONE_SHOT);
                }
            }
        }
    } 
}

void task_keypad_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        case KEYPAD_UPDATE_KEY_SIG: {
            keypad_read = get_key();
            if (keypad_read != last_keypad_read) {
                keypad_control_handle(keypad_read);
            }
            last_keypad_read = keypad_read;
            break;
        }
        default:
            break;
    }
}