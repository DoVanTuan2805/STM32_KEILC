#ifndef REC_H
#define REC_H
#include "Rec.h"
#include "stdint.h"
#include "Shape.h"
typedef struct
{
	Shape_t shape;
	uint16_t width;
	uint16_t height;
} Rec_t;

void Rec_ctor(Rec_t *const me, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif
