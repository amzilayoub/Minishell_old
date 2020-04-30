#include "../minishell.h"

int	check_error_spaces(char *arg, int *pos)
{
	int i;

	i = skip_spaces(arg);
	while (!pos[2] && arg[++i])
	{
		if (arg[i] == '\\')
			i++;
		else if (arg[i] == ' ')
			return (1);
	}
	return (0);
}

int	check_error_quotes(char *arg, int *pos)
{
	int i;

	i = -1;
	if (arg[skip_spaces(arg + pos[1]) + pos[1]])
		return (1);
	return (check_error_spaces(arg, pos));
}

void	replace_and_print(int oldpwd, char *dir)
{
	char	*starting_point;
	int	len;

	if (!oldpwd)
		return ;
	if (!(starting_point = ft_strnstr(dir, "/root", ft_strlen(dir))))
		ft_putstr(dir);
	else
	{
		len = ft_strlen("/root");
		if (!dir[len])
			ft_putstr("~");
		else
			ft_putstr(ft_strjoin("~", dir + ft_strlen("/root")));
	}
	ft_putstr("\n");
}

void	change_directory(char *dir, char ***envp)
{
	char		*tmp_dir;
	int		print_oldpwd;

	tmp_dir = ft_getcwd();
	if (dir[0] == '~')
		dir = ft_strjoin(get_env("HOME", *envp), dir + 1);
	else if (!dir[0] || !dir[skip_spaces(dir)])
		dir = get_env("HOME", *envp);
	else if ((print_oldpwd = (dir[0] == '-')))
	{
		if (dir[skip_spaces(dir + 1) + 1])
			error_and_exit("More than Two String\n");
		else
			dir = get_env("OLDPWD", *envp);
	}
	printf("DIR = |%s|\n", dir);
	if (chdir(dir) < 0)
		error_and_exit("No Such File or Directory !\n");
	replace_and_print(print_oldpwd, dir);
	ft_export(ft_strjoin("OLDPWD=", tmp_dir), envp);
	ft_export(ft_strjoin("PWD=", ft_getcwd()), envp);
}

void	ft_cd(char *arg, char ***envp)
{
	int	*pos;
	char	*param;
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '\\')
		{
			i++;
			continue;
		}
		else if (arg[i] == '$')
			arg = env_var_concat(arg, &i, *envp);
	}
	pos = find_quotes(arg);
	if (check_error_quotes(arg, pos))
		error_and_exit("More Than One String\n");
	remove_backslash(arg, pos);
	if (pos[2] && (arg[pos[0]] == '"' || arg[pos[0]] == '\''))
	{
		pos[0]++;
		pos[1] -= 2;
	}
	param = ft_substr(arg + pos[0], 0, pos[1] - pos[0] + 1);
	printf("BEFORE : OLDPWD = |%s| || PWD = |%s|\n", get_env("OLDPWD", *envp), get_env("PWD", *envp));
	change_directory(param, envp);
	printf("AFTER : OLDPWD = |%s| || PWD = |%s|\n", get_env("OLDPWD", *envp), get_env("PWD", *envp));
}
