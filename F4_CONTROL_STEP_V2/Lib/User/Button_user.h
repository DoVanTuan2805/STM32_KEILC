#ifndef BUTTON_USER_H
#define BUTTON_USER_H

#include "button.h"
#include "user_ex.h"

void initButton(void);
void buttonHandle(void);
void convertToArray1D(uint16_t **arr2D, uint16_t *arr1D, int rows, int cols);
void convertToArray2D(uint16_t *arr1D, uint16_t **arr2D, int rows, int cols);
#endif