#include "../minishell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int	_n_param(char *arg)
{
	int i;

	i = skip_spaces(arg);
	if (!ft_strncmp(arg + i, "-n", 2))
	{
		if (arg[i + 2] == ' ')
			return (i + 3);
	}
	return (0);
}

int	print_back_slash(int n)
{
	if (n > 2)
	{
		n = n / 3;
		while (n-- > 0)
			ft_putchar('\\');
	}
	else if (n == 2)
		ft_putchar('\\');
}

int	print_string(char *str)
{
	int i;
	int back_slash;
	int quote;
	int tmp;

	i = -1;
	quote = 0;
	back_slash = 0;
	while (str[++i])
	{
		if (!quote && str[i]== ' ' && (tmp = skip_spaces(str + i)))
		{
			i += tmp - 1;
			ft_putchar(' ');
		}
		else if (!quote && (str[i] == '"' || str[i] == '\'') && str[i - 1] != '\\')
			quote = str[i];
		else if (quote && str[i] == quote && str[i - 1] != '\\')
			quote = 0;
		else
		{
			if (str[i] == '\\')
			{
			/*
			**----------------------------------------------------------**
			** Problem of many back slash, for exemple : echo \\\\\\\\\\
			** i didn't code the part of back_slash yet
			**----------------------------------------------------------**
			*/
			
			}
			else
			{
				print_back_slash(back_slash);
				ft_putchar(str[i]);
			}
		}
	}
}

int	main(int argc, char **argv, char**envp)
{
	int	new_line;
	int	i;
	char	*param;
	int	quote;

	param = *argv;
	i = _n_param(param);
	new_line = !(i);
	i += skip_spaces(param + i);
	print_string(param + i);
	if (new_line)
		ft_putchar('\n');
}
