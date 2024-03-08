#include "Flash_user.h"
void Flash_Write_Struct(uint32_t address, dataSaveFlash_t dataUserWrite)
{
	Flash_Write_Array(address, (uint8_t *)&dataUserWrite, sizeof(dataUserWrite));
}
void Flash_Read_Struct(uint32_t address, dataSaveFlash_t *dataUserRead, uint16_t len)
{
	Flash_Read_Array(address, (uint8_t *)dataUserRead, len);
}

void Flash_Write_Struct2(uint32_t address, dataSaveFlash2_t dataUserWrite)
{
	Flash_Write_Array(address, (uint8_t *)&dataUserWrite, sizeof(dataUserWrite));
}
void Flash_Read_Struct2(uint32_t address, dataSaveFlash2_t *dataUserRead, uint16_t len)
{
	Flash_Read_Array(address, (uint8_t *)dataUserRead, len);
}
