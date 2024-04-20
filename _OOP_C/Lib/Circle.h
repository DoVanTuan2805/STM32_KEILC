#ifndef CIRCLE_H
#define CIRCLE_H

#include "stdint.h"
#include "Shape.h"
struct CircleVtable;

typedef struct
{
    Shape_t shape;
    uint16_t radius;
} Circle_t;

void Circle_ctor(Circle_t *const me, uint16_t x, uint16_t y, uint16_t radius);
#endif
