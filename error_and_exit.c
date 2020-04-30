# include "minishell.h"

void	error_and_exit(char *error)
{
	ft_putstr(error);
	exit(-1);
}
