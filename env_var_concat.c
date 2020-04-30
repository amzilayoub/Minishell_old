#include "minishell.h"

char	*get_env_var_helper(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\\')
		{
			i++;
			continue;
		}
		else if (str[i] == ' ' || str[i] == '\'' || str[i] == '"')
			break;
	}
	return (ft_substr(str, 1, i - 1));
}

char	*concat_param(char *arg, int i, char *env_var, int env_param_len)
{
	char	*new_str;
	char	*last_str;

	new_str = ft_substr(arg, 0, i);
	new_str = ft_strjoin(new_str, env_var);
	last_str = ft_substr(arg, i + env_param_len + 1, ft_strlen(arg + i + env_param_len - 1));
	new_str = ft_strjoin(new_str, last_str);
	return (new_str);
}

char	*env_var_concat(char *arg, int *i, char **envp)
{
	char	*start;
	char	*env_var;
	int	env_param_len;

	env_var = get_env_var_helper(arg + (*i));
	env_param_len = ft_strlen(env_var);
	remove_backslash(env_var, NULL);
	if (!(env_var = get_env(env_var, envp)))
		return (NULL);
	arg = concat_param(arg, *i, env_var, env_param_len);
	(*i) += (ft_strlen(env_var) - 2);
	return (arg);
}
