#ifndef CONFIG_H
#define CONFIG_H

#include "stdbool.h"
#include "stdint.h"

#define CIRCLE_RAD	(360)

typedef struct
{
	bool loop_mode_state;
}user_setting_t;

typedef struct
{
	user_setting_t user_setting;
} __attribute__((packed)) user_data_t;

extern user_data_t user_data;
#endif
