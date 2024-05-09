#ifndef __IO_CFG_H__
#define __IO_CFG_H__

#include "stdint.h"

extern uint8_t button_up_read();
extern uint8_t button_down_read();
extern void step_toggle();
extern void step_set_dir(uint8_t dir);
#endif

