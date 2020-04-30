# include "minishell.h"

int	*find_quotes(char *arg)
{
	int	i;
	int	*pos;

	pos = (int*)malloc(sizeof(int) * 3);
	i = -1;
	pos[0] = 0;
	pos[1] = ft_strlen(arg);
	pos[2] = 0;
	while (arg[++i])
	{
		if (!pos[2] && (arg[i] == '"' || arg[i] == '\''))
		{
			printf("XXXXXXXXXXXX =  %d\n", i);
			pos[0] = i;
			pos[2] = arg[i];
		}
		else if (pos[2] && pos[2] == arg[i])
		{
			pos[1] = i + 1;
			break;
		}
		if (arg[i] == '\\')
		{
			if (pos[2] && (arg[i + 1] == pos[2] || arg[i + 1] == '\\'))
				i++;
			else if (!pos[2])
				i++;
		}
	}
	return (pos);
}
