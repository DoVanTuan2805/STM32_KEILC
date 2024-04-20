#include "Circle.h"

#define pi 3

static void Circle_draw(Shape_t const *const shape);
static uint32_t Circle_area(Shape_t const *const shape);

void Circle_ctor(Circle_t *circle, 
				uint16_t x, 
				uint16_t y, 
				uint16_t rad)
{
	static struct ShapeVtable CircleVtable = {
		&Circle_draw,
		&Circle_area
	};
	Shape_ctor(&circle->shape, x, y);
	circle->shape.vptr = &CircleVtable;		// Point to CircleVtable
											// circle->shap.vptr->area = &Circle_draw
											// circle->shap.vptr->area = &Circle_area
	circle->rad = rad;
};
static void Circle_draw(Shape_t const *const shape)
{
	Circle_t const *const circle = (Circle_t const*)shape;
}
static uint32_t Circle_area(Shape_t const *const shape)
{
	Circle_t const *const circle = (Circle_t const*)shape;
	return (pi * (circle->rad * circle->rad));
}

