#ifndef __LCDUSER_H__
#define __LCDUSER_H__

#define NUM_OF_SCREEN 4
#include "stdbool.h"

typedef enum
{
  SCREEN1 = 0,
  SCREEN2 = 1,
  SCREEN3 = 2,
  SCREEN4 = 3,
} Screen_t;

bool ArrayScreen( Screen_t screen, int a, int b);
bool screen1(int a, int b);
bool screen2(int a, int b);
bool screen3(int a, int b);
bool screen4(int a, int b);

static bool (*handleScreen[NUM_OF_SCREEN])(int a, int b) = 
{
		screen1,
		screen2,
		screen3,
		screen4
};



#endif