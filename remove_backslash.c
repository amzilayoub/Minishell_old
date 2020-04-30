#include "minishell.h"

int	*fill_it(char *str)
{
	int *pos;

	pos = (int*)malloc(sizeof(int) * 3);
	pos[0] = 0;
	pos[1] = ft_strlen(str);
	pos[2] = 0;
	return (pos);
}

void	remove_backslash(char *param, int *pos)
{
	int start;
	int back_slash;
	int j;

	if (!pos)
		pos = fill_it(param);
	start = pos[0] - 1;
	pos[1] = (!pos[1]) ? ft_strlen(param) : pos[1];
	back_slash = 0;
	while (++start < pos[1] && param[start])
	{
		if (param[start] == '\\')
		{
			if (pos[2] && param[start + 1] != pos[2] && param[start + 1] != '\\')
				continue;
			j = start;
			while (++j < pos[1])
				param[j - 1] = param[j];
			param[j - 1] = '\0';
			pos[1]--;
		}
	}
}
