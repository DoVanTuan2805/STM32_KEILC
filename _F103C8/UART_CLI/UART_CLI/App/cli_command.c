#include "cli_command.h"

extern const cli_command_enty_t command_entry_table[];

const cli_command_func_t* find_command_infor (char *cmd)
{
		const cli_command_enty_t *command_entry = command_entry_table;
		while(command_entry->command_infor != NULL)
		{
				if(strcmp(command_entry->name, cmd) == 0)
				{
						return command_entry->command_infor;
				}
				command_entry++;
		}
		return NULL;
}


