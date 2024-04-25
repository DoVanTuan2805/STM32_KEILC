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
	TC_1_GPIO_Port, TC_2_GPIO_Port, TC_3_GPIO_Port, TC_4_GPIO_Port, TC_5_GPIO_Port, TC_6_GPIO_Port, TC_7_GPIO_Port, TC_8_GPIO_Port,	\
	TC_9_GPIO_Port, TC_10_GPIO_Port, TC_11_GPIO_Port, TC_12_GPIO_Port, TC_13_GPIO_Port, TC_14_GPIO_Port, TC_15_GPIO_Port, TC_16_GPIO_Port
};

uint16_t GPIO_Pin[NUM_CELL] = 
{
    TC_1_Pin, TC_2_Pin, TC_3_Pin, TC_4_Pin, TC_5_Pin, TC_6_Pin, TC_7_Pin, TC_8_Pin,			\
    TC_9_Pin, TC_10_Pin, TC_11_Pin, TC_12_Pin, TC_13_Pin, TC_14_Pin, TC_15_Pin, TC_16_Pin
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