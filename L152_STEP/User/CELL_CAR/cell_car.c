#include "cell_car.h"
#include "gpio.h"
#include "stdint.h"


/*
    PNP : 0 -> 6
    NPN : 7 -> 15

*/
#define EMPTY_PNP (0)
#define EMPTY_NPN (1)

#define PNP (7)
#define NPN (15)
bool arr_car_empty[NUM_CELL] =
    {
        1, 1, 1, 1,
        1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1
    };
GPIO_TypeDef *GPIOx[NUM_CELL] =
    {
	TC1_GPIO_Port, TC2_GPIO_Port, TC3_GPIO_Port, TC4_GPIO_Port, TC5_GPIO_Port, TC6_GPIO_Port, TC7_GPIO_Port, TC8_GPIO_Port,	\
	TC9_GPIO_Port, TC10_GPIO_Port, TC11_GPIO_Port, TC12_GPIO_Port, TC13_GPIO_Port, TC14_GPIO_Port, TC15_GPIO_Port, TC16_GPIO_Port
};

uint16_t GPIO_Pin[NUM_CELL] = 
{
    TC1_Pin, TC2_Pin, TC3_Pin, TC4_Pin, TC5_Pin, TC6_Pin, TC7_Pin, TC8_Pin,			\
    TC9_Pin, TC10_Pin, TC11_Pin, TC12_Pin, TC13_Pin, TC14_Pin, TC15_Pin, TC16_Pin
};

void read_E18() {
    for (int i = 0; i < NUM_CELL; i++)
    {
		arr_car_empty[i] = HAL_GPIO_ReadPin(GPIOx[i], GPIO_Pin[i]);
    }
}
int k = 0, f = 0;

uint8_t find_empty_car(bool cell_car[], uint8_t size_cell, uint8_t *vi_tri, uint8_t *soTang)
{
    for(k = 0; k < PNP; k++)
    {
        if(cell_car[k] == EMPTY_PNP)
        {
            if(k < 4)
            {
                *soTang = 0;
                if(k < 2)
                {
                    *vi_tri = k + 1;
                    return k;
                }
                else
                {
                    *vi_tri = k + 2;
                    return k;
                }
            }
            else {
                *soTang = 1;
                *vi_tri = k - (*soTang * 4) + 1;
                return k;
            }
        }
    }
    for(f = PNP; f < NPN; f++)
    {
        if(cell_car[f] == EMPTY_NPN)
        {
            if(f < 10)
            {
                *soTang = 1;
                *vi_tri = f - (*soTang * 4) + 1;
                return f;
            }
            else
            {
                *soTang = 2;
                *vi_tri = f - (*soTang * 4) - 1;
                return f;
            }
        }
    }
    return INT8_MAX;
}