#include "Circle.h"
static void _Circle_draw(Shape_t const *const me);
static uint32_t _Circle_area(Shape_t const *const me);
void Circle_ctor(Circle_t *const me, uint16_t x, uint16_t y, uint16_t radius)
{
    static struct shapeVtable CircleVtable = {
        &_Circle_draw,
        &_Circle_area
    };
    Shape_ctor(&me->shape, x, y);       
    me->shape.vptr = &CircleVtable;
    me->radius = radius;
}
static void _Circle_draw(Shape_t const *const me)
{
    Circle_t const *const me_ = (Circle_t const *)me;
}
static uint32_t _Circle_area(Shape_t const *const me){
    Circle_t const *const me_ = (Circle_t const *)me;
	return 3U * ((uint32_t)me_->radius * (uint32_t)me_->radius);
}   
