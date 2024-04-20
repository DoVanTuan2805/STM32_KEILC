#include "Rectangle.h"

static void Rectangle_draw(Shape_t const *const shape);
static uint32_t Rectangle_area(Shape_t const *const shape);

void Rectangle_ctor(Rectangle_t *const rect, 
					uint16_t x, 
					uint16_t y, 
					uint16_t width, 
					uint16_t height)
{
	static struct ShapeVtable RectVtable =			// DEFINE VIRTUAL TABLE RECTANGLE
	{
		&Rectangle_draw,
		&Rectangle_area
	};
	Shape_ctor(&rect->shape, x , y);
	rect->shape.vptr = &RectVtable;		// Point to Rectangle Table
										// rect->shap.vptr->area = &Rectangle_draw
										// rect->shap.vptr->area = &Rectangle_draw
	rect->height = height;
	rect->width = width;
};

static void Rectangle_draw(Shape_t const *const shape)
{
	Rectangle_t const *const rect = (Rectangle_t const*) shape;
}
static uint32_t Rectangle_area(Shape_t const *const shape)
{
	Rectangle_t const *const rect = (Rectangle_t const*) shape;
	return (rect->width * rect->height);
}
