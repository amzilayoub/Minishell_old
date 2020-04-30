# include "../minishell.h"

char	**create_new_array(char **envp)
{
	char	**new_envp;
	int	i;

	g_envp_count += ENVP_GROW;
	new_envp = (char**)malloc((g_envp_count + 1) * sizeof(char*));
	new_envp[g_envp_count] = NULL;
	i = -1;
	while (envp[++i])
		new_envp[i] = envp[i];
	return (new_envp);
}

void	add_variable(char *key, char *value, char ***envp)
{
	g_envp_last_index++;
	if (g_envp_count == g_envp_last_index)
		(*envp) = create_new_array(*envp);
	(*envp)[g_envp_last_index] = ft_strjoin(ft_strjoin(key, "="), value);
}

void	shift_char(char *arg)
{
	int i;

	i = -1;
	while (arg[++i])
		arg[i] = arg[i + 1];
}

char	*remove_quotes(char *param)
{
	int	i;
	char	quotes;

	i = -1;
	quotes = 0;
	while (param[++i])
	{
		if (param[i] == '\\')
		{
			i++;
			continue;
		}
		else if ((param[i] == '"' || param[i] == '\'') && quotes == param[i])
		{
			quotes = 0;
			shift_char(param + i);
		}
		else if ((param[i] == '"' || param[i] == '\'') && !quotes)
		{
			quotes = param[i];
			shift_char(param + i);
		}
	}
	return (param);
}

char	*get_env_key(char *arg)
{
	int i;
	char *key;
	int spaces;

	spaces = skip_spaces(arg);
	i = spaces - 1;
	if (arg[i + 1] == '$')
		return (NULL);
	while (arg[++i])
	{
		if (arg[i] == ' ')
			return (NULL);
		else if (arg[i] == '\\' && arg[i + 1] == '\\')
			return (NULL);
		else if (arg[i] == '=')
			break;
	}
	g_export_equal_index = i;
	if (!arg[i] || arg[i + 1] == '=')
		return (NULL);
	key = ft_substr(arg, spaces, i - spaces);
	key = remove_quotes(key);
	remove_backslash(key, NULL);
	return (key);
}

char	*get_value(char *arg, char **envp)
{
	int i;
	int quote;
	char *value;

	if ((i = skip_spaces(arg)))
	{
		g_export_equal_index += i;
		return (ft_strdup("\0"));
	}
	quote = 0;
	while (arg[++i])
	{
		if (arg[i] == '\\')
		{
			i++;
			continue;
		}
		else if ((arg[i] == '"' || arg[i] == '\'') && !quote)
			quote = arg[i];
		else if ((arg[i] == '"' || arg[i] == '\'') && quote == arg[i])
			quote = 0;
		else if (arg[i] == '$')
			arg = env_var_concat(arg, &i, envp);
		else if (arg[i] == ' ' && !quote)
			break;
	}
	g_export_equal_index += i + 1;
	value = remove_quotes(ft_substr(arg, 0, i));
	remove_backslash(value, NULL);
	return (value);
}

void	ft_export(char *arg, char ***envp)
{
	int i;
	char *env_key;
	char *env_value;
	int	key_len;

	i = -1;
	if (!(env_key = get_env_key(arg)))
		return ;
	env_value = get_value(arg + g_export_equal_index + 1, *envp);
	key_len = ft_strlen(env_key);
	while ((*envp)[++i])
	{
		if (!ft_strncmp((*envp)[i], env_key, key_len))
		{
			(*envp)[i] = ft_strjoin(env_key, ft_strjoin("=", env_value));
			break ;
		}
	}
	if (!(*envp)[i])
		add_variable(env_key, env_value, envp);
	g_export_equal_index += skip_spaces(arg + g_export_equal_index);
	if (arg[g_export_equal_index])
		ft_export(arg + g_export_equal_index, envp);
}
