#include "Response_CI.h"

static UART_HandleTypeDef *handleUart;

void ResponseInit(UART_HandleTypeDef *Uart)
{
		handleUart = Uart;
}

void Response_print(const char *str, ...)
{
		char stringArray[100];
		
		va_list args;
		va_start(args, str);
		vsprintf(stringArray, str, args);
		va_end(args);
		
		HAL_UART_Transmit(handleUart, (uint8_t*)stringArray, strlen((char*)stringArray), 100);
}

/*
{  
"iccid": "89882390000143872813", 
 "modbusDeviceAddress": 1, 
 "modbusRegisterStartingAddress": 70,  
 "systemProfileId": "07848739-8eaf-4139-a74b-ca6f96afba58",  
 "value": 50.0, 
 "recordedTimestamp": "2024-01-25T04:30:07Z"
 }
 */