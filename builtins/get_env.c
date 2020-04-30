# include "../minishell.h"

char	*get_env(char *needle, char **env)
{
	int	i;
	char	*res;

	i = -1;
	while (env[++i])
	{
		res = ft_split(env[i], '=')[0];
		if (!ft_strcmp(res, needle))
			return (ft_substr(env[i] + ft_strlen(res), 1, ft_strlen(env[i])));
	}
	return (NULL);
}
