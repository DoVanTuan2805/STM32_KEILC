#include "UART.h"
#include "getTemp_CI.h"
#include "setTemp_CI.h"
#include "cli_command.h"
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
				char *argv[10];
				uint8_t argvNum = 0;
				data = strtok((char*)UartBuff, " ");
				
				while(data != NULL)
				{
						argv[argvNum] = data;
						data = strtok(NULL, " ");
						argvNum++;
				}
				const cli_command_infor_t *command_infor = find_command_infor(argv[0]);
				
				if(command_infor != NULL)
				{
						command_infor->function(argv, argvNum);
				}
//				if(strcmp((char*)argv[0], "getTemperature") == 0)
//				{
//						getTemp((char**)argv, argvNum);
//				}
//				else if(strcmp((char*)argv[0], "setTemperatureMax") == 0)
//				{
//						setTempMax((char**)argv, argvNum);
//				}
//				else if(strcmp((char*)argv[0], "setTemperatureMin") == 0)
//				{
//						setTempMin((char**)argv, argvNum);
//				}
				
				memset(UartBuff, 0, strlen((char*)UartBuff));
				UartLen = 0;
				UartFlag = 0;
		}
}

