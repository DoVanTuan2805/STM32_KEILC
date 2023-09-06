#ifndef CLI_TYPES_H
#define CLI_TYPES_H

#include "stdio.h"
#include "stdint.h"
typedef void (*cli_command_func_t)(char **argv, uint8_t argvNum);

typedef struct
{
		cli_command_func_t function;
		char *help;
}cli_command_infor_t;

typedef struct
{
		const char *name;
		const cli_command_infor_t *command_infor;
}cli_command_enty_t;

#endif