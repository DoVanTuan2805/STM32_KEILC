#include "setTemp_CI.h"
#include "Response_CI.h"
void setTempMax(char **argv, uint8_t argvNum)
{
		if(argvNum < 3)
		{
				Response_print("Error dont enough argument");
		}
		else if(argvNum > 3)
		{
				Response_print("Error too much argument");
		}
		Response_print();
}

void setTempMin(char **argv, uint8_t argvNum)
{
		
}