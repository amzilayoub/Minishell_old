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

void	fill_pipe_fd(int nb_pipe)
{
	int i;

	i = -1;
	while (++i < nb_pipe)
	{
		g_pipe_fd[i] = (int*)malloc(sizeof(int) * 2);
	}
}

void	shell_loop(char **envp)
{
	char	*line;
	int	i;
	int	status;
	int	nb_pipe;
	int	proc_called;

	proc_called = -1;
	waiting_new_cmd();
	dup2(1, 10);
	while (get_next_line(0, &line) > 0)
	{
		proc_called = -1;
		nb_pipe = calc_pipe(line);
		prepare_line(line, nb_pipe);
		if (!(g_pipe_fd = (int**)malloc(sizeof(int*) * nb_pipe)))
			printf("==================== MALLOC ERROR ======================\n");
		fill_pipe_fd(nb_pipe);
		i = -1;
		while (g_cmd_call[++i].cmd)
		{
			proc_called++;
			if (proc_called < nb_pipe)
				pipe(g_pipe_fd[proc_called]);
			g_pid = fork();
			if (g_pid == 0)
			{
				/*
				** for Writing
				*/
				if (nb_pipe && g_cmd_call[i + 1].cmd)
				{
					//printf("IF %d\n\n", i + 1);
					//close(1);
					dup2(g_pipe_fd[proc_called][1], 1);
					close(g_pipe_fd[proc_called][1]);
				}
				else if (!g_cmd_call[i + 1].cmd)
				{
					//printf("ELSE IF %d\n\n", i + 1);
					//close(1);
					dup2(g_stdio_fd[1], 1);
				}
				/*
				** for reading
				*/
				if (i != 0)
				{
					//printf("IF i != 0\n", i + 1);
					//close(0);
					dup2(g_pipe_fd[proc_called - 1][0], 0);
					close(g_pipe_fd[proc_called - 1][0]);
				}
				//if (proc_called < nb_pipe)
					//close(g_pipe_fd[proc_called][0]);
				if (!i)
				{
					write(1, g_cmd_call[i].param_line, ft_strlen(g_cmd_call[i].param_line));
					//printf("FIRST CMD %s\n", "We Can");
					exit(1);
				}
				else
					execve(ft_strjoin("builtins/", g_cmd_call[i].cmd), &g_cmd_call[i].param_line, envp);
				printf("minishell :( : ERROR\n");
				exit(-1);
			}
			else
			{
				wait(&status);
				if (proc_called < nb_pipe)
					close(g_pipe_fd[proc_called][1]);
			}
			/*
			if (nb_pipe)
			{
				nb_pipe--;
				pipe(g_pipe_fd[0]);
				pipe(g_pipe_fd[1]);
			}
			g_pid = fork();
			if (g_pid == 0)
			{
				close(g_pipe_fd[0][0]);
				dup2(g_pipe_fd[0][1], 1);
				close(g_pipe_fd[0][1]);
				execve(ft_strjoin("builtins/", g_cmd_call[i].cmd), &g_cmd_call[i].param_line, envp);
				printf("CHILD 1\n");
				exit(1);
			}
			else if (g_pid < 0)
				printf("ERROR\n");
			else
			{
				wait(&status);
				g_pid = fork();
				if (g_pid == 0)
				{
					dup2(g_pipe_fd[0][0], 0);
					//close(1);
					close(g_pipe_fd[0][0]);
					dup2(g_pipe_fd[1][1], 1);
					close(g_pipe_fd[1][1]);
					//char s[51];
					//read(0, s, 50);
					execve(ft_strjoin("builtins/", g_cmd_call[i + 1].cmd), &g_cmd_call[i + 1].param_line, envp);
					//printf("CHILD 2 = %s\n", s);
					exit(1);
				}
				else
				{
					wait(&status);
					g_pid = fork();
					if (g_pid == 0)
					{
						//close(1);
						dup2(g_pipe_fd[1][0], 0);
						close(g_pipe_fd[1][0]);
						dup2(g_stdio_fd[1], 1);
						//char s[51];
						//read(0, s, 50);
						//printf("CHILD 3 = %s\n", s);
						//exit(1);
						execve(ft_strjoin("builtins/", g_cmd_call[i + 2].cmd), &g_cmd_call[i + 2].param_line, envp);
					}
					else
						wait(&status);
					dup2(g_stdio_fd[0], 0);
				}
				//printf("PARENT\n");
			}
			break;
			*/
			/*
			if (nb_pipe)
			{
				nb_pipe--;
				pipe(g_pipe_fd);
				dup2(g_pipe_fd[0], 0);
				dup2(g_pipe_fd[1], 1);
			}
			else
				dup2(g_stdio_fd[1], 1);
			*/
			/*
			printf("CMD = %s\n", g_cmd_call[j].cmd);
			printf("PARAM = %s\n", (g_cmd_call[j].param_line));
			printf("-----------------------------------\n");
			*/
		}
		//close(g_pipe_fd[0]);
		dup2(g_stdio_fd[0], 0);
		waiting_new_cmd();
	}
}
