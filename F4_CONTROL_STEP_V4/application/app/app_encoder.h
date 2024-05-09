#ifndef __APP_ENCODER_H__
#define __APP_ENCODER_H__

#include "encoder.h"

extern encoder_t encoder_left;
extern encoder_t encoder_right; 

extern void encoder_left_callback(void* e);
extern void encoder_right_callback(void* e);
#endif