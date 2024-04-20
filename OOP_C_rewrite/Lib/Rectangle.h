#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Shape.h"
#include "stdint.h"
typedef struct
{
	Shape_t shape;
	uint16_t width;
	uint16_t height;
}Rectangle_t;

void Rectangle_ctor(Rectangle_t *const rect, 
					uint16_t x, 
					uint16_t y, 
					uint16_t width, 
					uint16_t height);

#endif

