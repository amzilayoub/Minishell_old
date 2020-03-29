# include "minishell.h"

int	calc_pipe(char *line)
{
	int i;
	int quote;
	int pipe;

	quote = 0;
	pipe = 0;
	i = -1;
	while (line[++i])
	{
		/*
		**--------------------------------------------**
		**  check if there's the open quote or double quotes
		**--------------------------------------------**
		*/
		if ((line[i] == '\'' || line[i] == '"') && !quote && line[i - 1] != '\\')
			quote = line[i];
		else if ((line[i] == '\'' || line[i] == '"') && quote == line[i] && line[i - 1] != '\\')
			quote = 0;
		pipe += (line[i] == '|' && line[i - 1] != '\\' && !quote);
	}
	return (pipe);
}

void    cmd_position(char *line)
{
        int i;
        int quote;
        int j;

        quote = 0;
        i = -1;
        j = -1;
        g_cmd_call[j = 0].cmd_pos = 0;
        while (line[++i])
        {
                /*
                **--------------------------------------------**
                **  check if there's the open quote or double quotes
                **--------------------------------------------**
                */
                if ((line[i] == '\'' || line[i] == '"') && !quote && line[i - 1] != '\\')
                        quote = line[i];
                else if ((line[i] == '\'' || line[i] == '"') && quote == line[i] && line[i - 1] != '\\')
                        quote = 0;
                if (line[i] == '|' && line[i - 1] != '\\' && !quote)
                	g_cmd_call[++j].cmd_pos = i;
        }
        g_cmd_call[++j].cmd = 0;
}


int	count_word(char *line)
{
	int i;

	i = 0;
	while (line[i] && ft_isalpha(line[i]))
		i++;
	return (i);
}

int	get_param_line(char *line, int start, int next_pipe)
{
	int len;

	next_pipe = (g_cmd_call[next_pipe].cmd_pos) ? g_cmd_call[next_pipe].cmd_pos : ft_strlen(line);
	len = next_pipe - start;
	return (len <= 0 ? 0 : len);
}

void	prepare_line(char *line, int nb_pipe)
{
	int i;
	int j;
	int len;

	i = -1;
	/*
	**----------------------------------------------------**
	** add  for the first command
	**----------------------------------------------------**
	*/
	nb_pipe += 2;
	g_cmd_call = (t_cmd*)malloc(sizeof(t_cmd) * nb_pipe);
	cmd_position(line);
	while (++i < nb_pipe - 1)
	{
		j = g_cmd_call[i].cmd_pos - 1;
		while (line[++j])
		{
			//if (g_cmd_call[i + 1].cmd && i >= g_cmd_call[i + 1].cmd_pos)
				//break;
			if (line[j] != ' ' && line[j] != '|')
			{
				len = count_word(line + j);
				g_cmd_call[i].cmd = ft_substr(line, j, len);
				j += len + 1;
				len = get_param_line(line, j, i + 1);
				g_cmd_call[i].param_line = ft_substr(line, j, len);
				break;
			}
		}
	}
}

void	shell_loop(char **envp)
{
	char	*line;
	int	i;
	int	status;
	int	nb_pipe;
	int	pipe_count;

	pipe_count = 0;
	waiting_new_cmd();
	while (get_next_line(0, &line) > 0)
	{
		prepare_line(line, nb_pipe = calc_pipe(line));
		i = -1;
		while (g_cmd_call[++i].cmd)
		{
			if (nb_pipe)
			{
				nb_pipe--;
				pipe(g_pipe_fd);
				dup2(g_pipe_fd[0], 0);
				dup2(g_pipe_fd[1], 1);
			}
			else
				dup2(g_stdio_fd[1], 1);
			if (!(g_pid = fork()))
			{
				execve(ft_strjoin("builtins/", g_cmd_call[i].cmd), &g_cmd_call[i].param_line, envp);
				ft_putstr_fd("minishell: ERROR\n", 2);
				exit(-1);
			}
			else if (g_pid < 0)
				ft_putstr_fd("ERROR\n", 2);
			else
				wait(&status);
			/*
			printf("CMD = %s\n", g_cmd_call[j].cmd);
			printf("PARAM = %s\n", (g_cmd_call[j].param_line));
			printf("-----------------------------------\n");
			*/
		}
		close(g_pipe_fd[0]);
		dup2(g_stdio_fd[0], 0);
		waiting_new_cmd();
	}
}
