#ifndef FLASH_H
#define FLASH_H

#include "stdint.h"

typedef enum
{
		U8,
		U16
}TypeArray_t;

void Flash_Write_Int(uint32_t address, int value);
void Flash_Write_Float(uint32_t address, float value);
void Flash_Write_Array(uint32_t address, uint8_t *arrayWrite, uint16_t len);

int Flash_Read_Int(uint32_t address);
float Flash_Read_Float(uint32_t address);
void Flash_Read_Array(uint32_t address, uint8_t *arrayRead, uint16_t len);

#endif