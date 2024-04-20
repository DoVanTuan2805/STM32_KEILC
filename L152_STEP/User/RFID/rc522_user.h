#ifndef RC522_USER_H
#define RC522_USER_H

#include "stdbool.h"
#include "stdint.h"
void rc522_init();
bool read_RC522_In();
bool read_RC522_Out();

#endif