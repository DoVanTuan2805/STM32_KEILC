#include "stm32f10x.h"
#include "Rectangle.h"
#include "Circle.h"
#include "stdio.h"
int main(void)
{
	Rectangle_t r1, r2, r3;
	Circle_t c1, c2, c3;
	Shape_t const* shapeArray[] = {
		&r1.shape,
		&c1.shape,
		&r2.shape,
		&c2.shape,
		&r3.shape,
		&c3.shape
	};
	Rectangle_ctor(&r1,1 , 2, 3, 4); 
	Rectangle_ctor(&r2,10 , 20, 30, 40); 
	Rectangle_ctor(&r3,15 , 20, 35, 45); 
	
	Circle_ctor(&c1, 5, 5, 10);
	Circle_ctor(&c2, 10, 10, 20);
	Circle_ctor(&c3, 15, 15, 30);
	drawAllShape(shapeArray);

	return 0;
}
