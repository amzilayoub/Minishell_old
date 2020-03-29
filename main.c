#include "minishell.h"

void	waiting_new_cmd(void)
{
	write(1, ">>> ", 4);
}

int	main(int argc, char **argv, char **envp)
{
	g_stdio_fd[0] = dup(0);
	g_stdio_fd[1] = dup(1);
	if (g_stdio_fd[0] < 0 || g_stdio_fd[1] < 0)
	{
		ft_putstr_fd("minishell: ERROR\n", 2);
		exit(-1);
	}
	init_cmd_str();
	shell_loop(envp);
	return (0);
}
