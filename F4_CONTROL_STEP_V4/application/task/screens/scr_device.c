#include "ak.h"
#include "timer.h"

#include "math.h"

#include "app.h"
#include "task_list.h"

#include "task_keypad.h"
#include "scr_device.h"

#include "view_manager.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "logo.h"

#define SETTING_TEXT_NUM (5)
static const char *setting_text[SETTING_TEXT_NUM] = {"SETTING", "LOOP MODE      :", "PULSE              :", "SPEED (0 > 9)    :", "INDEX (0 > 360) :"};

uint16_t angle_config[FLOOR_MAX][ANGLE_FLOOR_MAX];

user_step_info_t user_step_info;
screen_info_t screen_info;
uint8_t last_screen;

void reset_config_angle() {
	for(int i = 0; i < FLOOR_MAX; i++)
	{
		for (int j = 0; j < ANGLE_FLOOR_MAX;j++){
			angle_config[i][j] = 0;
		}
	}
	task_post_pure_msg(TASK_FLASH_ID, FLASH_WRITE_CFG_ANGLE_SIG);
}
void view_variable_init()
{
	// VARIABLE SETTING
	// user_step_info.user_setting.loopmode = false;
	// user_step_info.user_setting.pulse = 3200;
	// user_step_info.user_setting.speed = 8;
	// user_step_info.user_setting.index = 10;
	// task_post_pure_msg(TASK_FLASH_ID, FLASH_WRITE_SETTING_SIG);

	//reset_config_angle();

	// VARIABLE CONFIG ANGLE
	user_step_info.user_config_angle.floor = 0;
	user_step_info.user_config_angle.page = 0;
	

	// VARIABLE SCREN MAIN
	user_step_info.user_scr_main.step_deg = 0;							// 
	user_step_info.user_scr_main.cheater = 0;
	user_step_info.user_scr_main.floor = 0;
	user_step_info.user_scr_main.ptr_tble_angle = 0	;

	screen_info.screen_option = SCREEN_OPTION_RUN;			// SCREEN MAIN

	screen_info.screen_main_option = SCREEN_MAIN_ENCODER_HAND;
	screen_info.screen_setting_option = SCREEN_SETTING_NULL;
}

/*
	state : 0 -> CELL SETTING
	state : 1 -> CELL CONFIG ANGLE
*/
void view_scr_cell(uint8_t state) {
#define CELL_NUM (5)
#define CELL_HEIGHT (46)
#define CELL_WIDTH (ILI9341_SCREEN_WIDTH - 10)
#define CELL_X (5)
	ILI9341_Draw_Hollow_Rectangle_Coord(5, 5, ILI9341_SCREEN_WIDTH - 5 ,ILI9341_SCREEN_HEIGHT - 5, WHITE);
        for (uint8_t i = 0; i < CELL_NUM; i++) {
            if (state == 0) {
                ILI9341_Draw_Horizontal_Line(CELL_X, 5 + (CELL_HEIGHT * i),
                                             CELL_WIDTH, WHITE);
            } 
			else {
				ILI9341_Draw_Horizontal_Line(CELL_X + 75, 5 + (CELL_HEIGHT * i),
                                             CELL_WIDTH - 75, WHITE);
            }
        }
}

/*----------------------------------- SCREEN MAIN --------------------------------- */
// CIRCLE MODE ENCODER 
void view_main_circle() {
	#define CIRCLE_CENTER_X (215)
	#define CIRCLE_CENTER_Y (135)
	float sx, sy; // Saved H, M, S x & y multipliers
	static float osx = 120, osy = 121; // Saved H, M, S x & y coords
	uint16_t x0, x1, yy0, yy1;

	ILI9341_Draw_Text("ANGLE:", CIRCLE_CENTER_X - 30, 10 , WHITE, BLACK, Font1);
	
	ILI9341_Draw_Filled_Rectangle_Coord(CIRCLE_CENTER_X + 25, 10, CIRCLE_CENTER_X + 60, 25, BLACK);
	ILI9341_Draw_Int((user_step_info.user_scr_main.step_deg / (CIRCLE_RAD / user_step_info.user_setting.index)), CIRCLE_CENTER_X + 25, 10 , WHITE, BLACK, Font1);
	
	ILI9341_Draw_Hollow_Circle(CIRCLE_CENTER_X, CIRCLE_CENTER_Y, 93, WHITE);
	//ILI9341_Draw_Filled_Circle(CIRCLE_CENTER_X, CIRCLE_CENTER_Y, 3, WHITE);

	for (float i = 0; i < 360; i += (CIRCLE_RAD / user_step_info.user_setting.index))
	{
		sx = cos((i - 90) * 0.0174532925);
		sy = sin((i - 90) * 0.0174532925);
		x0 = sx * 90 + CIRCLE_CENTER_X;	
		yy0 = sy * 90 + CIRCLE_CENTER_Y;
		x1 = sx * 86 + CIRCLE_CENTER_X;
		yy1 = sy * 86 + CIRCLE_CENTER_Y;
		ILI9341_Draw_Line(x0, yy0, x1, yy1, YELLOW);
	}

	sx = cos((user_step_info.user_scr_main.step_deg - 90) * 0.0174532925);
	sy = sin((user_step_info.user_scr_main.step_deg - 90) * 0.0174532925);


	ILI9341_Draw_Filled_Circle(osx, osy, 3, BLACK);

	osx = sx * 78 + CIRCLE_CENTER_X; // 	Y : 200 -> position X CIRCLE
	osy = sy * 78 + CIRCLE_CENTER_Y; //	Y : 140 -> position Y CIRCLE

	ILI9341_Draw_Filled_Circle(osx, osy, 3, RED);

	if(data[0] != '\0')
	{
		ILI9341_Draw_Filled_Rectangle_Coord(CIRCLE_CENTER_X, 25, CIRCLE_CENTER_X + 60, 40, BLACK);
		ILI9341_Draw_Text(data, CIRCLE_CENTER_X, 25 , WHITE, BLACK, Font1);
	}
	else 
	{
		ILI9341_Draw_Filled_Rectangle_Coord(CIRCLE_CENTER_X, 25, CIRCLE_CENTER_X + 60, 40, BLACK);
	}
}

// OPTION IN MAIN (CHEATER, FLOOR, TABLE ANGLE)
void view_main_option()
{
	// Y: 5-> 75
#define TEXT_X (15)
#define NUM_X	(85)

#define NUM_CLEAR_X1	(80)
#define NUM_CLEAR_X2	(115)
	ILI9341_Draw_Text("INDEX    :", TEXT_X, 10 , WHITE, BLACK, Font1);
	ILI9341_Draw_Filled_Rectangle_Coord( NUM_CLEAR_X1, 10, NUM_CLEAR_X2, 35, BLACK);
	ILI9341_Draw_Int(user_step_info.user_setting.index, NUM_X, 10 , GREEN, BLACK, Font1);

	ILI9341_Draw_Text("CHEATER:", TEXT_X, 35 , WHITE, BLACK, Font1);
	ILI9341_Draw_Filled_Rectangle_Coord( NUM_CLEAR_X1, 35, NUM_CLEAR_X2, 60, BLACK);
	ILI9341_Draw_Float(user_step_info.user_scr_main.cheater, NUM_X, 35 , GREEN, BLACK, Font1);
	if (user_step_info.user_scr_main.cheater < -0.1) {
		ILI9341_Draw_Line(82, 42, 87, 42, GREEN);
	}

    ILI9341_Draw_Text("FLOOR   :", TEXT_X, 60 , WHITE, BLACK, Font1);
	ILI9341_Draw_Filled_Rectangle_Coord( NUM_CLEAR_X1, 60, NUM_CLEAR_X2, 85, BLACK);
	ILI9341_Draw_Int(user_step_info.user_scr_main.floor + 1, NUM_X, 60 , GREEN, BLACK, Font1);
}

// TABLE ANGLE IN MAIN
void view_main_table_angle()		
{
	// Y: 70 -> 235 = 165
#define TEXT_ANGLE_X (15)
#define NUM_ANGLE_X	(45)
#define TRIANGLE_X		(20)

#define ANGLE_CLEAR_X1	(6)
#define ANGLE_CLEAR_Y1	(86)
#define ANGLE_CLEAR_X2	(109)
#define ANGLE_CLEAR_Y2	(234)

#define FLOOR_TBL_ANGLE				(user_step_info.user_scr_main.floor)
#define SLOT_TBL_ANGLE(i)			(user_step_info.user_scr_main.ptr_tble_angle + (i - 2))
#define SLOT_MAX					(19)
#define SLOT_MIN					(0)

#define INDEX_TBL_ANGLE_X			(14)
#define INDEX_TBL_ANGLE_Y			(177)

	ILI9341_Draw_Filled_Rectangle_Coord( ANGLE_CLEAR_X1, ANGLE_CLEAR_Y1, ANGLE_CLEAR_X2, ANGLE_CLEAR_Y2, BLACK);
	ILI9341_Draw_Text("TABLE ANGLE", TEXT_ANGLE_X, 100 , WHITE, BLACK, Font1);
	for (int i = 0; i < 5; i++) {
		if ( SLOT_TBL_ANGLE(i) < SLOT_MIN || SLOT_TBL_ANGLE(i) > SLOT_MAX) {
			continue;
		}
		if (i == 2) {
			ILI9341_Draw_Int(
				angle_config[FLOOR_TBL_ANGLE][SLOT_TBL_ANGLE(i)],
				NUM_ANGLE_X, 130 + (i * 20), RED, BLACK, Font1);
			continue;
		}
		ILI9341_Draw_Int(
			angle_config[FLOOR_TBL_ANGLE][SLOT_TBL_ANGLE(i)],
			NUM_ANGLE_X, 130 + (i * 20), GREEN, BLACK, Font1);
    }
	ILI9341_Draw_Vertical_Line(INDEX_TBL_ANGLE_X + 25, INDEX_TBL_ANGLE_Y - 15, 30, GREEN);
	ILI9341_Draw_Int(user_step_info.user_scr_main.ptr_tble_angle + 1, INDEX_TBL_ANGLE_X, INDEX_TBL_ANGLE_Y - 8, RED, BLACK, Font1);
	// ILI9341_Draw_Filled_Triangle_Coord(TRIANGLE_X, 170, TRIANGLE_X, 190, TRIANGLE_X + 20, 180, RED); 
}

void view_main_cell()
{
	ILI9341_Draw_Hollow_Rectangle_Coord(5, 5, ILI9341_SCREEN_WIDTH - 5 ,ILI9341_SCREEN_HEIGHT - 5, WHITE);
	ILI9341_Draw_Vertical_Line(115, 5, ILI9341_SCREEN_HEIGHT - 10, WHITE);
	ILI9341_Draw_Horizontal_Line(5, 85, 110, WHITE);
}

void view_scr_main()
{
	view_main_circle();
	view_main_cell();
	view_main_option();
	view_main_table_angle();
}

void view_scr_main_option()
{
	switch (screen_info.screen_main_option)
	{
	case SCREEN_MAIN_ENCODER_HAND:
		ILI9341_Draw_Text("ANGLE:", CIRCLE_CENTER_X - 30, 10 , RED, BLACK, Font1);
		break;

	case SCREEN_MAIN_CHEATER_HAND:
		ILI9341_Draw_Text("CHEATER:", TEXT_X, 35 , RED, BLACK, Font1);
		break;

	case SCREEN_MAIN_FLOOR_HAND:
		ILI9341_Draw_Text("FLOOR   :", TEXT_X, 60 , RED, BLACK, Font1);
		break;

	case SCREEN_MAIN_ANGLE_HAND:
		ILI9341_Draw_Text("TABLE ANGLE", TEXT_ANGLE_X, 100 , RED, BLACK, Font1);
		break;

	default:
		break;
	}
}
/*----------------------------------- END SCREEN MAIN --------------------------------- */


/*----------------------------------- SCREEN SETTING --------------------------------- */
static inline void view_scr_setting_parameter(uint8_t i) {
#define PARAMETER_X 			(140)
#define PARAMETER_Y(i)			(5 + (CELL_HEIGHT * i) + 15)	

#define PARAMETER_CLEAR_X		(PARAMETER_X - 5)
#define PARAMETER_CLEAR_Y(i)	(5 + (CELL_HEIGHT * i) + 10)

#define CELL_HEIGHT (46)

    ILI9341_Draw_Filled_Rectangle_Coord(PARAMETER_CLEAR_X, PARAMETER_Y(i),
                                        PARAMETER_CLEAR_X + 130,
                                        PARAMETER_Y(i) + 20, BLACK);
    if (i == 1) {
        ILI9341_Draw_Text(user_step_info.user_setting.loopmode == 1 ? "ON " : "OFF",
                          PARAMETER_X, PARAMETER_Y(i), WHITE, BLACK, Font1);
    } else if (i == 2) {
        ILI9341_Draw_Int(user_step_info.user_setting.pulse, PARAMETER_X,
                         PARAMETER_Y(i), WHITE, BLACK, Font1);
    } else if (i == 3) {
        ILI9341_Draw_Int(user_step_info.user_setting.speed, PARAMETER_X,
                         PARAMETER_Y(i), WHITE, BLACK, Font1);
    } else if (i == 4) {
        ILI9341_Draw_Int(user_step_info.user_setting.index, PARAMETER_X,
                         PARAMETER_Y(i), WHITE, BLACK, Font1);
    }
}

void view_scr_setting()
{
#define CELL_NUM 	(5)
#define TITLE_X		(130)
#define TEXT_X		(15)
#define TEXT_Y		(15)

	view_scr_cell(0);
	for (uint8_t i = 0; i < CELL_NUM; i++) {
		if(i != 0) {
        	ILI9341_Draw_Text(setting_text[i], TEXT_X, 5 + (CELL_HEIGHT * i) + 15, WHITE, BLACK, Font1);
			view_scr_setting_parameter(i);
		}
		else 
		{
			ILI9341_Draw_Text(setting_text[i], 130, 5 + (CELL_HEIGHT * i) + 15, GREEN, BLACK, Font1);
		}
	}
}

void view_scr_setting_option()
{
	switch (screen_info.screen_setting_option)
	{
	case SCREEN_SETTING_LOOPMODE:
		ILI9341_Draw_Text(setting_text[1], TEXT_X, 5 + (CELL_HEIGHT * 1) + 15, RED, BLACK, Font1);
		break;

	case SCREEN_SETTING_PULSE:
		ILI9341_Draw_Text(setting_text[2], TEXT_X, 5 + (CELL_HEIGHT * 2) + 15, RED, BLACK, Font1);
		break;

	case SCREEN_SETTING_SPEED:
		ILI9341_Draw_Text(setting_text[3], TEXT_X, 5 + (CELL_HEIGHT * 3) + 15, RED, BLACK, Font1);
		break;

	case SCREEN_SETTING_INDEX:
		ILI9341_Draw_Text(setting_text[4], TEXT_X, 5 + (CELL_HEIGHT * 4) + 15, RED, BLACK, Font1);
		break;

	default:
		break;
	}
}
/*----------------------------------- END SCREEN SETTING --------------------------------- */

/*----------------------------------- SCREEN CONFIG ANGLE --------------------------------- */
void view_scr_config_angle() {
#define FLOOR_CONFIG_ANGLE_X1 (6)
#define FLOOR_CONFIG_ANGLE_Y1 (6)
#define FLOOR_CONFIG_ANGLE_X2 (80)
#define FLOOR_CONFIG_ANGLE_Y2 (234)

#define FLOOR_TEXT_X (15)
#define FLOOR_TEXT_Y (100)
#define FLOOR_NUM_X (25)
#define FLOOR_NUM_Y (120)

#define CELL_NUM (5)
#define CELL_HEIGHT (46)

#define ANGLE_TEXT_X 			(105)
#define ANGLE_TEXT_Y(i) 		(5 + (CELL_HEIGHT * i) + 15)

#define PARAMETER_ANGLE_CLEAR_X 		(ANGLE_TEXT_X + 50)
#define PARAMETER_ANGLE_CLEAR_Y(i) 		(5 + (CELL_HEIGHT * i) + 10)

#define PARAMETER(i) (i + 1 + ((user_step_info.user_config_angle.page) * CELL_NUM))

#define FLOOR				(user_step_info.user_config_angle.floor)
#define ANGLE_IN_FLOOR(i)	(i + ((user_step_info.user_config_angle.page) * CELL_NUM))
#define ANGLE_X				(200)
#define ANGLE_Y(i)			((5 + (CELL_HEIGHT * i) + 15))

    view_scr_cell(1);  // 5 cell
    ILI9341_Draw_Filled_Rectangle_Coord(
        FLOOR_CONFIG_ANGLE_X1, FLOOR_CONFIG_ANGLE_Y1, FLOOR_CONFIG_ANGLE_X2,
        FLOOR_CONFIG_ANGLE_Y2, BLACK);

    ILI9341_Draw_Vertical_Line(FLOOR_CONFIG_ANGLE_X2, FLOOR_CONFIG_ANGLE_Y1,
                               FLOOR_CONFIG_ANGLE_Y2 - 4, WHITE);

    ILI9341_Draw_Text("FLOOR :", FLOOR_TEXT_X, FLOOR_TEXT_Y, RED, BLACK, Font1);
    ILI9341_Draw_Int(user_step_info.user_config_angle.floor + 1, FLOOR_NUM_X,
                     FLOOR_NUM_Y, RED, BLACK, Font1);

    for (uint8_t i = 0; i < CELL_NUM; i++) {
        if (user_step_info.user_config_angle.slot == i) {
            ILI9341_Draw_Text("ANGLE      :", ANGLE_TEXT_X, ANGLE_TEXT_Y(i),
                              RED, BLACK, Font1);

            // CLEAR PARAMETER SLOT
            ILI9341_Draw_Filled_Rectangle_Coord(
                PARAMETER_ANGLE_CLEAR_X, PARAMETER_ANGLE_CLEAR_Y(i),
                PARAMETER_ANGLE_CLEAR_X + 20, PARAMETER_CLEAR_Y(i) + 25, BLACK);

            ILI9341_Draw_Int(PARAMETER(i), ANGLE_TEXT_X + 45, ANGLE_TEXT_Y(i),
                             RED, BLACK, Font1);
        } 
		else {
            ILI9341_Draw_Text("ANGLE      :", ANGLE_TEXT_X, ANGLE_TEXT_Y(i),
                              GREEN, BLACK, Font1);

            // CLEAR PARAMETER SLOT
            ILI9341_Draw_Filled_Rectangle_Coord(
                PARAMETER_ANGLE_CLEAR_X, PARAMETER_ANGLE_CLEAR_Y(i),
                PARAMETER_ANGLE_CLEAR_X + 20, PARAMETER_CLEAR_Y(i) + 25, BLACK);

            ILI9341_Draw_Int(PARAMETER(i), ANGLE_TEXT_X + 45, ANGLE_TEXT_Y(i),
                             GREEN, BLACK, Font1);
        }
        // CLEAR PARAMETER ANGLE IN SLOT
        ILI9341_Draw_Filled_Rectangle_Coord(
            ANGLE_X - 5, ANGLE_Y(i) - 5, ANGLE_X + 20, ANGLE_Y(i) + 25, BLACK);

        ILI9341_Draw_Int(angle_config[FLOOR][ANGLE_IN_FLOOR(i)], ANGLE_X,
                         ANGLE_Y(i), GREEN, BLACK, Font1);
    }
}

/*----------------------------------- END SCREEN CONFIG ANGLE --------------------------------- */

void view_clear_change_option() {
    if (screen_info.screen_option != last_screen) {
		if(last_screen == SCREEN_OPTION_CONFIG_ANGLE)
		{
			task_post_pure_msg(TASK_FLASH_ID, FLASH_WRITE_CFG_ANGLE_SIG);
			task_post_pure_msg(TASK_FLASH_ID, FLASH_READ_CFG_ANGLE_SIG);
		} 
		else if (last_screen == SCREEN_OPTION_SETTING)
		{
			task_post_pure_msg(TASK_FLASH_ID, FLASH_WRITE_SETTING_SIG);
			task_post_pure_msg(TASK_FLASH_ID, FLASH_READ_SETTING_SIG);
		}
        last_screen = screen_info.screen_option;
        task_post_pure_msg(TASK_SCREEN_DEVICE_ID, SCREEN_CHANGE);
    }
}

void view_scr_option()
{
	switch(screen_info.screen_option)
	{
		case SCREEN_OPTION_RUN:
			view_scr_main();
			view_scr_main_option();			
			break;
		case SCREEN_OPTION_CONFIG_ANGLE:
			view_scr_config_angle();
			break;
		case SCREEN_OPTION_SETTING:
			view_scr_setting();
			view_scr_setting_option();
			break;
	}
}

void task_screen_device_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        case SCREEN_SETUP_SIG: {
			user_step_info.user_setting.loopmode = false;
            view_variable_init();
            screen_clear();
			config_angle_reset_parameter();
			timer_set(TASK_KEYPAD_ID, KEYPAD_UPDATE_KEY_SIG, AC_KEYPAD_TIMER_INTERVAL, TIMER_PERIODIC);
            timer_set(TASK_SCREEN_DEVICE_ID, SCREEN_RUN_SIG,
                      AC_DISPLAY_ENCODER_HAND_TIMER_INTERVAL, TIMER_ONE_SHOT);
			
         	break;
		}

        case SCREEN_RUN_SIG: {
			ILI9341_Draw_Bitmap(137, 57, (int16_t *)logo, LOGO_WIDTH, LOGO_HEIGHT);
            view_scr_option();
            break;
        }

        case SCREEN_MAIN_CHANGE_OPTION_SIG: {
            view_scr_option();
            break;
        }

		case SCREEN_SETTING_CHANGE_OPTION_SIG: {
			view_scr_option();
			break;
		}

		case SCREEN_CONFIG_ANGLE_CHANGE_SIG:
			view_scr_option();
			break;

		case SCREEN_CHANGE: {
			screen_clear();
			if(screen_info.screen_option == SCREEN_OPTION_RUN)
			{
				ILI9341_Draw_Bitmap(138, 56, (int16_t *)logo, LOGO_WIDTH, LOGO_HEIGHT);
			}
			break;
		}			
    }
}