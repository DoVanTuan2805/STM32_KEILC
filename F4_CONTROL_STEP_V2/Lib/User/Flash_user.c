#include "Flash_user.h"

void Flash_Write_Struct_Setting(uint32_t address, dataSaveFlash_t dataUserWrite)
{
	Flash_Write_Array(address, (uint16_t *)&dataUserWrite, sizeof(dataUserWrite));
}
void Flash_Read_Struct_Setting(uint32_t address, dataSaveFlash_t *dataUserRead, uint16_t len)
{
	Flash_Read_Array(address, (uint16_t *)dataUserRead, len);
}
void Flash_Write_Struct_Rotation(uint32_t address, dataPage_t dataUserWrite)
{
	Flash_Write_Array(address, (uint16_t *)&dataUserWrite, sizeof(dataUserWrite));
}
void Flash_Read_Struct_Rotation(uint32_t address, dataPage_t *dataUserRead, uint16_t len)
{
	Flash_Read_Array(address, (uint16_t *)dataUserRead, len);
}