#include "cli_command_table.h"
#include "cli_types.h"
#include "getTemp_CI.h"

const cli_command_infor_t command_infor_getTemp=
{
		getTemp,
		"Get Temp from Sensor"
};

const cli_command_enty_t command_entry_table[] =
{
	{"getTemperature", &command_infor_getTemp},
};