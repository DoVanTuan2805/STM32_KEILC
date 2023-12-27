#ifndef __LCD_USER_H
#define __LCD_USER_H

#include "i2c-lcd.h"
#include "i2c.h"


#define NUM_OF_SCREEN 4
#include "stdbool.h"

typedef enum
{
  SCREEN_NON_LOGIN = 0,
  SCREEN_CHECK_INOUT = 1,
  SCREEN_WAIT_CHECKIN = 2,
  SCREEN_WAIT_CHECKOUT = 3,
} Screen_t;

bool ArrayScreen( Screen_t screen);

void lcdInit(void);
void lcdNonLogin(void);
void lcdCheckInOut(void);
void waitCheckIn(void);
void waitCheckOut(void);

void lcdHanle(void);

static void ( *handleScreen[NUM_OF_SCREEN] )= 
{
		lcdNonLogin,
		lcdCheckInOut,
		waitCheckIn,
		waitCheckOut
};


#endif