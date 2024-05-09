#ifndef KEYPAD_USER_H
#define KEYPAD_USER_H

#include "stdint.h"

#include "KEYPAD.h"
#include "KEYPAD_cfg.h"

#define KEY_1       0
#define KEY_2       1
#define KEY_3       2
#define KEY_F1      3
#define KEY_4       4
#define KEY_5       5
#define KEY_6       6
#define KEY_F2      7
#define KEY_7       8
#define KEY_8       9
#define KEY_9       10
#define KEY_F3      11
#define KEY_A       12
#define KEY_0       13
#define KEY_H       14
#define KEY_F4      15

extern char key[16];
extern uint8_t u8_KeyStatesArr[KEYS];

extern void keypad_handler();
extern void keypad_init();
extern char getKey();
#endif