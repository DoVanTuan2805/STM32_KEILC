#include "Shape.h"
#include "assert.h"
static void _Shape_draw(Shape_t const *const shape);
static uint32_t _Shape_area(Shape_t const *const shape);
void Shape_ctor(Shape_t *const shape, uint16_t x, uint16_t y)
{
	struct ShapeVtable vtable = {
		&_Shape_draw,
		&_Shape_area
	};
	shape->vptr = &vtable;
	shape->x = x;
	shape->y = y;
}
void drawAllShape(Shape_t const* shape[])
{
	for (int i = 0; i < sizeof(shape); i++)
	{

		Shape_draw(shape[i]);
	}
}
static void _Shape_draw(Shape_t const *const shape)
{
	assert(0);
}
static uint32_t _Shape_area(Shape_t const *const shape)
{
	assert(0);
	return 0U;
}
