#include "flash.h"
#include "stm32f4xx_hal.h"

uint32_t getSector(uint32_t address)
{
	if (address >= 0x08000000 && address < 0x08003FFF)
	{
		return FLASH_SECTOR_0;
	}
	else if (address >= 0x08004000 && address < 0x08007FFF)
	{
		return FLASH_SECTOR_1;
	}
	else if (address >= 0x08008000 && address < 0x0800BFFF)
	{
		return FLASH_SECTOR_2;
	}
	else if (address >= 0x0800C000 && address < 0x0800FFFF)
	{
		return FLASH_SECTOR_3;
	}
	else if (address >= 0x08010000 && address < 0x0801FFFF)
	{
		return FLASH_SECTOR_4;
	}
	else if (address >= 0x08020000 && address < 0x0803FFFF)
	{
		return FLASH_SECTOR_5;
	}
	else if (address >= 0x08040000 && address < 0x0805FFFF)
	{
		return 6;
	}
	else if (address >= 0x08060000 && address < 0x0807FFFF)
	{
		return 7;
	}
}

void Flash_Write_Int(uint32_t address, int value)
{
	HAL_FLASH_Unlock();
	uint32_t sector = getSector(address);
	FLASH_Erase_Sector(sector, FLASH_VOLTAGE_RANGE_3);
	HAL_Delay(50);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, value);
	HAL_FLASH_Lock();
}
void Flash_Write_Float(uint32_t address, float value)
{
	uint8_t data[4];
	*(float *)data = value;
	HAL_FLASH_Unlock();
	uint32_t sector = getSector(address);
	FLASH_Erase_Sector(sector, FLASH_VOLTAGE_RANGE_3);
	HAL_Delay(50);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *(uint64_t *)data);
	HAL_FLASH_Lock();
}
void Flash_Write_Array(uint32_t address, uint16_t *arrayWrite, uint16_t len)
{
	HAL_FLASH_Unlock();
	uint32_t sector = getSector(address);
	FLASH_Erase_Sector(sector, FLASH_VOLTAGE_RANGE_3);
	HAL_Delay(50);
	uint16_t *pt = (uint16_t *)arrayWrite;
	for (int i = 0; i < (len + 1) / 2; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + 2 * i, *pt); // increase address 2byte
		pt++;
	}
	HAL_FLASH_Lock();
}
int Flash_Read_Int(uint32_t address)
{
	return *(__IO uint16_t *)address;
}
float Flash_Read_Float(uint32_t address)
{
	uint32_t data = *(__IO uint32_t *)address;
	return *(float *)(&data);
}
void Flash_Read_Array(uint32_t address, uint16_t *arrayRead, uint16_t len)
{
	uint16_t *pt = (uint16_t *)arrayRead;
	for (int i = 0; i < (len + 1) / 2; i++)
	{
		*pt = *(__IO uint16_t *)(address + 2 * i); // increase address 2byte
		pt++;
	}
}
