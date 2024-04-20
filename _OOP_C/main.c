#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "Rec.h"
#include "Circle.h"
static void GPIO_config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef gpioInitStruct;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioInitStruct.GPIO_Pin = GPIO_Pin_2;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &gpioInitStruct);
}
/*
static void UART_config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef UsartInitStruct;
	UsartInitStruct.USART_BaudRate = 9600;
	UsartInitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UsartInitStruct.USART_Parity = USART_Parity_No;
	UsartInitStruct.USART_WordLength = USART_WordLength_8b;
	UsartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UsartInitStruct.USART_StopBits = USART_StopBits_1;

	USART_Init(USART1, &UsartInitStruct);
}
*/
static void delay(int ms)
{
    while (ms--)
        ;
}
int main(void)
{
    Rec_t r1, r2; /* multiple instances of Rectangle */
    Circle_t c1, c2; /* multiple instances of Circle */

    Shape_t const *shapeArray[] = { /* collection of shapes */
		&c2.shape,
        &c1.shape,
        &r2.shape,
        &r1.shape
    };
	
	/* instantiate rectangles... */
    Rec_ctor(&r1, 0, 2, 10, 15);
    Rec_ctor(&r2, 1, 3, 5, 8);

    /* instantiate circles... */
    Circle_ctor(&c2, 1, 3, 22);
    Circle_ctor(&c1, 1, 2, 12);
	
    drawAllShapes(shapeArray);
    
    GPIO_config();
	//UART_config();
    while (1)
    {
        //Shape_ctor(&s1, 1, 2);
        //Shape_moveBy(&s1, 3, 4);
		//Rec_ctor(&r1, 5 ,6 , 8, 9);
        GPIO_SetBits(GPIOB, GPIO_Pin_2);
        delay(1000000);
        GPIO_ResetBits(GPIOB, GPIO_Pin_2);
        delay(1000000);
    }
}
