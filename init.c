#include "minishell.h"

void	init_cmd_str(void)
{
	g_cmd_str[0] = "echo";
	g_cmd_str[1] = "cd";
	g_cmd_str[2] = "pwd";
	g_cmd_str[3] = "export";
	g_cmd_str[4] = "unset";
	g_cmd_str[5] = "env";
	g_cmd_str[6] = "exit";
	g_cmd_str[7] = NULL;
}

