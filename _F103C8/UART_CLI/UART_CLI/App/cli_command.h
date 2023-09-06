#ifndef CLI_COMMAND_H
#define CLI_COMMAND_H
#include "cli_types.h"
#include "cli_command_table.h"
#include "string.h"
const cli_command_func_t* find_command_infor (char *cmd);

#endif

