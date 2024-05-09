#ifndef __APP_BSP_H__
#define __APP_BSP_H__

#include "button.h"

extern button_t button_up;
extern button_t button_down;

extern void btn_down_callback(void* b);
extern void btn_up_callback(void* b);
#endif
