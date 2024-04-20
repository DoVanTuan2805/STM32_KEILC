#ifndef SHAPE_H
#define SHAPE_H
#include "stdint.h"

struct ShapeVtable;
typedef struct
{

	struct ShapeVtable const *vptr;		// virtual table
	uint16_t x;
	uint16_t y;
}Shape_t;	

/*	VIRTUAL TABLE	*/
struct ShapeVtable
{
	void		(*draw) (Shape_t const *const shape);
	uint32_t 	(*area) (Shape_t const * const shape);
};

/*	VIRTUAL CALL */
static inline void Shape_draw(Shape_t const *const shape)
{
	(*shape->vptr->draw) (shape);
}
static inline uint32_t Shape_area(Shape_t const *const shape)
{
	return (*shape->vptr->area) (shape);
}

void Shape_ctor(Shape_t *const shape, uint16_t x, uint16_t y);
void drawAllShape(Shape_t const* shape[]);
#endif
