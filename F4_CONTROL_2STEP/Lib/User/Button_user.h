#ifndef BUTTON_USER_H
#define BUTTON_USER_H

#include "button.h"

typedef enum 
{
	MANUAL_SCREEN = 0,
	AUTO_SCREEN		= 1,
}Screen_t;



void initButton(void);
void buttonHandle(void);

#endif