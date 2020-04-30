#include "minishell.h"

void	init_cmd_str(void)
{
	g_cmd_str[0] = "cd";
	g_cmd_str[1] = "export";
	g_cmd_str[2] = "unset";
	g_cmd_str[3] = "exit";
	g_cmd_str[4] = 0;
}

void	init_var(char **envp)
{
}

void	init_cmd_fun(void)
{
	g_cmd_fun[0] = &ft_cd;
	g_cmd_fun[1] = &ft_export;
	g_cmd_fun[2] = &ft_unset;
	g_cmd_fun[3] = &ft_exit;
}

void	init(char **envp)
{
	init_var(envp);
	init_cmd_str();
	init_cmd_fun();
}
