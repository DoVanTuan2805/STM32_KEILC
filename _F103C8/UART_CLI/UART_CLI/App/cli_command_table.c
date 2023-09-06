#include "cli_command_table.h"
#include "cli_types.h"
#include "getTemp_CI.h"
#include "setTemp_CI.h"
const cli_command_infor_t command_infor_getTemp=
{
		getTemp,
		"Get Temp from Sensor"
};

const cli_command_infor_t command_infor_setTempMax=
{
		setTempMax,
		"Set Temp Max"
};

const cli_command_infor_t command_infor_setTempMin=
{
		setTempMin,
		"Set Temp Min"
};


const cli_command_enty_t command_entry_table[] =
{
	{"getTemperature", &command_infor_getTemp},
	{"setTemperatureMax", &command_infor_setTempMax},
	{"setTemperatureMin", &command_infor_setTempMin},
	{NULL, NULL}
};