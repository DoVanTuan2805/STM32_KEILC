#include "Shape.h"
#include "assert.h"
static void _Shape_draw(Shape_t const * const me);
static uint32_t _Shape_area(Shape_t const * const me);
void Shape_ctor(Shape_t *const me, uint16_t x, uint16_t y)
{
	static struct shapeVtable const vtble = {
		&_Shape_draw,
		&_Shape_area
	};
	me->vptr = &vtble;
	me->x = x;
	me->y = y;
}
void Shape_moveBy(Shape_t *const me, uint16_t dx, uint16_t dy)
{
	me->x += dx;
	me->y += dy;
}
uint16_t Shape_getX(Shape_t *const me)
{
	return me->x;
}
uint16_t Shape_getY(Shape_t *const me)
{
	return me->y;
}
void drawAllShapes(Shape_t const *shapes[])
{
	uint32_t i;
    for (i = 0U; i < sizeof(shapes); ++i) {
        Shape_draw(shapes[i]); /* virtual call */
    }
}
static void _Shape_draw(Shape_t const * const me)
{
	(void)me;
	assert(0);
}
static uint32_t _Shape_area(Shape_t const * const me)
{
	(void)me;
	assert(0);
	return 0U;
}
