#ifndef _SCR_ENCODER_HAND_H_
#define _SCR_ENCODER_HAND_H_

#include "stdint.h"

#define CIRCLE_RAD 	(360)
#define SPEED_MAX 	(9)
#define INDEX_MAX 	(CIRCLE_RAD)

#define FLOOR_MAX			(4)
#define ANGLE_FLOOR_MAX		(20)
#define ANGLE_PAGE_MAX	(4)

#define CHEATER_STEP	(0.1)


typedef enum 
{
	SCREEN_OPTION_RUN,
	SCREEN_OPTION_CONFIG_ANGLE,
	SCREEN_OPTION_SETTING,
}screen_option_t;

typedef enum 
{
	SCREEN_MAIN_ENCODER_HAND,
	SCREEN_MAIN_CHEATER_HAND,
	SCREEN_MAIN_FLOOR_HAND,
	SCREEN_MAIN_ANGLE_HAND,
	SCREEN_MAIN_NULL,
}screen_main_option_t;

typedef enum 
{
	SCREEN_SETTING_LOOPMODE,
	SCREEN_SETTING_PULSE,
	SCREEN_SETTING_SPEED,
	SCREEN_SETTING_INDEX,
	SCREEN_SETTING_NULL,
}screen_setting_option_t;

typedef struct
{
	screen_option_t 		screen_option;
	screen_main_option_t 	screen_main_option;
	screen_setting_option_t screen_setting_option;
}screen_info_t;

typedef struct {
    bool loopmode;
    uint8_t speed;
    uint16_t index;
    uint32_t pulse;
} user_setting_t;

typedef struct {
    uint8_t floor;
	int8_t page;
	int8_t slot;
} user_config_angle_t;

typedef struct 
{
	int8_t ptr_tble_angle;
	int8_t floor;
	volatile float cheater;
	volatile float step_deg;
}user_scr_main_t;


typedef struct 
{
	user_setting_t 		user_setting;
	user_config_angle_t user_config_angle;
	user_scr_main_t		user_scr_main;
	
}user_step_info_t;

extern screen_info_t screen_info;
extern user_step_info_t user_step_info;
extern uint8_t last_screen;
extern uint16_t angle_config[FLOOR_MAX][ANGLE_FLOOR_MAX];

extern void view_clear_change_option();
#endif

