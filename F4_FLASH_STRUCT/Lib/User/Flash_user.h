#include "flash.h"
//#include "ILI9431_user.h"
#include "stdint.h"

typedef struct
{
	uint8_t *data;
	uint8_t value;
} dataSaveFlash_t;

typedef struct
{
	uint8_t *data;
	uint8_t value;
} dataSaveFlash2_t;

void Flash_Write_Struct(uint32_t address, dataSaveFlash_t dataUserWrite);
void Flash_Read_Struct(uint32_t address, dataSaveFlash_t *dataUserRead, uint16_t len);

void Flash_Write_Struct2(uint32_t address, dataSaveFlash2_t dataUserWrite);
void Flash_Read_Struct2(uint32_t address, dataSaveFlash2_t *dataUserRead, uint16_t len);