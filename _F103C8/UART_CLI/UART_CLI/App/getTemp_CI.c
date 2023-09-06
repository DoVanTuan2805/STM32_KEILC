#include "getTemp_CI.h"

static uint8_t channel;
float temp;
void getTemp(char **argv, uint8_t argvNum)			// char **argv -> array matrix -> char *array[10]
{
		if(argvNum < 2)
		{
				Response_print("Error dont enough argument !"); 
				return;
		}
		else if(argvNum > 2)
		{
				Response_print("Error too much argument !"); 
				return;
		}
		Response_print("Ok\n");
		channel = atoi(argv[1]);			// CONVERT ARRAY STRING TO INT
		Response_print("command: %s\nchannel: %d\n", argv[0], channel);
		temp = getTempChannel(channel);
}
