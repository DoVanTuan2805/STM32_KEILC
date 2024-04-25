#ifndef LCD_USER_H
#define LCD_USER_H

#include "stdint.h"

#include "config.h"

#define NUM_OF_SCREEN 8
typedef enum
{
    SCREEN_ENCOER_STEP,
	SCREEN_CHEATER,
    SCREEN_FLOOR_HAND,
	SCREEN_ANGLE_HAND,

    SCREEN_SETTING,
} Screen_t;

extern Screen_t screen_state;

extern void lcd_init();
extern void screen_encoder_step(user_data_t user_data);
extern void screen_cheater(user_data_t user_data);
extern void screen_floor_hand(user_data_t user_data);
extern void screen_angle_hand(user_data_t user_data);
extern void screen_setting(user_data_t user_data);
	

extern uint8_t ArrayTFT(Screen_t screen, user_data_t user_data);

#endif