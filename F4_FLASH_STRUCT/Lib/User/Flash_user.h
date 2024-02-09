#include "flash.h"
//#include "ILI9431_user.h"
#include "stdint.h"

typedef struct
{
	uint8_t value;
	char Speed[10];
	char Pulse[10];
} dataSaveFlash_t;

void Flash_Write_Struct(uint32_t address, dataSaveFlash_t dataUserWrite);
void Flash_Read_Struct(uint32_t address, dataSaveFlash_t *dataUserRead, uint16_t len);
