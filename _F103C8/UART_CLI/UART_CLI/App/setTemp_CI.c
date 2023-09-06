#include "setTemp_CI.h"
#include "Response_CI.h"
#include "stdlib.h"
void setTempMax(char **argv, uint8_t argvNum)
{
		if(argvNum < 3)
		{
				Response_print("Error dont enough argument\n");
				return;
		}
		else if(argvNum > 3)
		{
				Response_print("Error too much argument\n");
				return;
		}
		Response_print("Ok\n");
		uint8_t channel = atoi(argv[1]);
		uint8_t value = atoi(argv[2]);
		Response_print("command: %s\nchannel: %d\nvalue: %d\n", argv[0], channel,value);
}

void setTempMin(char **argv, uint8_t argvNum)
{
		if(argvNum < 3)
		{
				Response_print("Error dont enough argument\n");
				return;
		}
		else if(argvNum > 3)
		{
				Response_print("Error too much argument\n");
				return;
		}
		Response_print("Ok\n");
		uint8_t channel = atoi(argv[1]);
		uint8_t value = atoi(argv[2]);
		Response_print("command: %s\nchannel: %d\nvalue: %d\n", argv[0], channel,value);
}