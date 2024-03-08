#include "flash.h"
#include "ILI9431_user.h"
#include "stdint.h"
#include "user_ex.h"

void Flash_Write_Struct_Setting(uint32_t address, dataSaveFlash_t dataUserWrite);
void Flash_Read_Struct_Setting(uint32_t address, dataSaveFlash_t *dataUserRead, uint16_t len);

void Flash_Write_Struct_Rotation(uint32_t address, dataPage_t dataUserWrite);
void Flash_Read_Struct_Rotation(uint32_t address, dataPage_t *dataUserRead, uint16_t len);
