# include "../minishell.h"

void	shift_env(char **envp)
{
	int i;

	i = -1;
	while (envp[++i])
		envp[i] = envp[i + 1];
}

void	remove_env_var(char **env_var, int len_env_var, char ***envp)
{
	int i;
	int j;

	i = -1;
	while ((*envp)[++i])
	{
		j = -1;
		while (env_var[++j])
		{
			if (!ft_strncmp((*envp)[i], env_var[j], ft_strlen(env_var[j])))
			{
				shift_env((*envp) + i);
				len_env_var--;
				g_envp_last_index--;
			}
			if (!len_env_var)
				break;
		}
	}
}

void	ft_unset(char *arg, char ***envp)
{
	int i;
	char **arguments;

	i = skip_spaces(arg);
	if (!(arg[i]))
	{
		printf("Error in number of argument");
		return ;
	}
	arguments = ft_split(arg + i, ' ');
	i = -1;
	while (arguments[++i])
	{
		arguments[i] = remove_quotes(arguments[i]);
		remove_backslash(arguments[i], NULL);
	}
	remove_env_var(arguments, i, envp);
}
