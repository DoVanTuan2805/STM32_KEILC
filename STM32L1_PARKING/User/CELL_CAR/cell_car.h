#ifndef CELL_CAR_H
#define CELL_CAR_H
#include "stdint.h"
#include "stdbool.h"

#define NUM_CELL (16)

extern bool arr_car_empty[NUM_CELL];

void read_E18();
uint8_t find_empty_car(bool cell_car[], uint8_t size_cell, uint8_t *vi_tri, uint8_t *soTang);
#endif