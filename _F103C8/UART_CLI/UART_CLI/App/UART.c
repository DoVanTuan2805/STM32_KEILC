#include "UART.h"

#define MAX_DATA_UART 100

static uint8_t UartBuff[MAX_DATA_UART];
static uint8_t UartLen;
static uint8_t UartFlag;

void ReceiveData(uint8_t dataRx)
{
		if(dataRx == '\n')		// rx complete
		{
				UartBuff[UartLen++] = '\0';
				UartFlag = 1;
		}
		else 
		{
				UartBuff[UartLen++] = dataRx;
		}
}
void UartHandle(void)
{
		if(UartFlag == 1)
		{			
				char *data;
				uint8_t *argv[10];
				uint8_t argvNum = 0;
				data = strtok((char*)UartBuff, " ");
				
				while(data != NULL)
				{
						argv[argvNum] = (uint8_t*)data;
						data = strtok(NULL, " ");
						argvNum++;
				}
				
				if(strcmp((char*)argv[0], "getTemperature") == 0)
				{
						getTemp((char**)argv, argvNum);
				}
				memset(UartBuff, 0, strlen((char*)UartBuff));
				UartLen = 0;
				UartFlag = 0;
		}
}

