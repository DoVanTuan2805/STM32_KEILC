#ifndef BUTTON_USER_H
#define BUTTON_USER_H

#include "button.h"

typedef enum 
{
		ENCODER_ROTATION_SCREEN,
		HAND_ROTATION_SCREEN,
	
		ENCODER_ANGLE_SCREEN,
		HAND_ANGLE_SCREEN,
		
		SETUP_SCREEN
}Screen_t;

void initButton(void);
void buttonHandle(void);

#endif