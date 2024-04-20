#include "Rec.h"
#include "stdio.h"
static void _Rec_draw(Shape_t const *const me);
static uint32_t _Rec_area(Shape_t const *const me);
void Rec_ctor(Rec_t *const me, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	static struct shapeVtable rectVtable = {
		&_Rec_draw,
		&_Rec_area
	};
	Shape_ctor(&me->shape, x, y);
	me->shape.vptr = &rectVtable;
	me->width = width;
	me->height = height;
}
static void _Rec_draw(Shape_t const *const me)
{
	Rec_t const *const me_ = (Rec_t const *)me;
	/*
	printf("Rec : x: %d - y:%d - width:%d - height:%d", 
		Shape_getX(me), 
		Shape_getY(me),
		me_->width, 
		me_->height
	);
	*/
}
static uint32_t _Rec_area(Shape_t const *const me)
{
	Rec_t const *const me_ = (Rec_t const *)me;
	return ((uint32_t)me_->width * (uint32_t)me_->height);
}
