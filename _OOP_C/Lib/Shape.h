#ifndef SHAPE_H
#define SHAPE_H

#include "stdint.h"
struct shapeVtable;
typedef struct
{
	struct shapeVtable const *vptr;
	uint16_t x;
	uint16_t y;
} Shape_t;

struct shapeVtable
{
	void (*draw)(Shape_t const *const me);
	uint32_t (*area)(Shape_t const *const me);
};

void Shape_ctor(Shape_t *const me, uint16_t x, uint16_t y);
void Shape_moveBy(Shape_t *const me, uint16_t dx, uint16_t dy);
uint16_t Shape_getX(Shape_t *const me);
uint16_t Shape_getY(Shape_t *const me);
void drawAllShapes(Shape_t const *shapes[], uint32_t nShapes);


static inline void Shape_draw(Shape_t const * const me) {
	// Rectangle_draw(me);
	// Circle_draw(me);
   (*me->vptr->draw)(me);
}
static inline uint32_t Shape_area(Shape_t const * const me) {
	return (*me->vptr->area)(me);
}

#endif
