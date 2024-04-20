#ifndef CIRCLE_H
#define CIRCLE_H

#include "stdint.h"
#include "Shape.h"
typedef struct
{
	Shape_t shape;
	uint16_t rad;
}Circle_t;
void Circle_ctor(Circle_t *circle, 
				uint16_t x, 
				uint16_t y, 
				uint16_t rad);
#endif
