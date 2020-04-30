# include "minishell.h"

int	calc_pipe(char *line)
{
	int i;
	int quote;

	quote = 0;
	g_nb_pipe = 0;
	g_nb_semicolons = 0;
	g_nb_redirections = 0;
	i = -1;
	while (line[++i])
	{
		/*
		**--------------------------------------------**
		**  check if there's the open quote or double quotes
		**--------------------------------------------**
		*/
		if (line[i] == '\\')
		{
			i++;
			continue;
		}
		else if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if ((line[i] == '\'' || line[i] == '"') && quote == line[i])
			quote = 0;
		g_nb_redirections += ((line[i] == '>' || line[i] == '<') && !quote);
		g_nb_pipe += (line[i] == '|' && !quote);
		g_nb_semicolons += (line[i] == ';' && !quote);
		i += (!ft_strncmp(line + i, ">>", 2)) ? 1 : 0;
	}
	return (g_nb_pipe);
}

void    cmd_position(char *line)
{
        int i;
        int quote;
        int j;

        quote = 0;
        i = -1;
        g_cmd_call[j = 0].cmd_pos = 0;
        while (line[++i])
        {
                /*
                **--------------------------------------------**
                **  check if there's the open quote or double quotes
                **--------------------------------------------**
                */
		if (line[i] == '\\')
		{
			i++;
			continue;
		}
		else if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if ((line[i] == '\'' || line[i] == '"') && quote == line[i])
			quote = 0;
                if ((line[i] == '|' || line[i] == ';' || line[i] == '>' || line[i] == '<') && !quote)
                {
                	if (line[i] == '>' || line[i] == '<')
                	{
                		g_cmd_call[++j].cmd_pos = i;
                		i += (!ft_strncmp(line + i, ">>", 2)) ? 1 : 0;
                	}
                	else
                		g_cmd_call[++j].cmd_pos = i + 1;
                	g_cmd_call[j].is_semicolon = (line[i] == ';');
                }
        }
        g_cmd_call[++j].cmd = 0;
}


int	count_word(char *line)
{
	int i;

	i = 0;
	while (line[i] && ft_isalpha(line[i]))
		i++;
	if (i == 0 && (line[i] == '>' || line[i] == '<'))
	{
		if (!ft_strncmp(line, ">>", 2))
			return (2);
		else
			return (1);
	}
	return (i);
}

int	get_param_line(char *line, int start, int next_pipe)
{
	int len;
	int nn = next_pipe;

	next_pipe = (g_cmd_call[next_pipe].cmd_pos) ? g_cmd_call[next_pipe].cmd_pos - 1: ft_strlen(line);
	len = next_pipe - start;
	return (len <= 0 ? 0 : len);
}

void	init_cmd_array()
{
	int i;

	i = -1;
	while (++i < g_nb_pipe + g_nb_semicolons + g_nb_redirections)
	{
		g_cmd_call[i].cmd = 0;
		g_cmd_call[i].param_line = 0;
		g_cmd_call[i].param_array = 0;
		g_cmd_call[i].cmd_pos = -1;
		g_cmd_call[i].is_semicolon = 0;
	}
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
	g_cmd_call = (t_cmd*)malloc(sizeof(t_cmd) * (nb_pipe + g_nb_semicolons + g_nb_redirections));
	init_cmd_array();
	cmd_position(line);
	//printf("SEM = %d | PIPE = %d\n", g_nb_semicolons, g_nb_pipe);
	while (++i < nb_pipe + g_nb_semicolons + g_nb_redirections - 1)
	{
		j = g_cmd_call[i].cmd_pos;
		//printf("POS = %d\n", j);
		j += skip_spaces(line + j);
		//printf("NUM = %d\n", j);
		if (line[j] == ';' || line[j] == '|')
		{
			// set_error here
			g_cmd_call[i].cmd = ";";
			continue;
		}
		//while (line[++j])
		{
			//if (g_cmd_call[i + 1].cmd && i >= g_cmd_call[i + 1].cmd_pos)
				//break;
			if (line[j] != ' ' && line[j] != '|' && line[j] != ';')
			{
				len = count_word(line + j);
				g_cmd_call[i].cmd = ft_substr(line, j, len);
				j += len + 1;
				len = get_param_line(line, j, i + 1);
				g_cmd_call[i].param_line = ft_substr(line, j, len);
				//break;
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
	int	j;
	int	status;
	int	proc_called;
	char	*cur_dir;

	cur_dir = ft_strjoin(ft_getcwd(), "/builtins/");
	proc_called = -1;
	waiting_new_cmd();
	while (get_next_line(0, &line) > 0)
	{
		proc_called = -1;
		calc_pipe(line);
		prepare_line(line, g_nb_pipe);
		if (!(g_pipe_fd = (int**)malloc(sizeof(int*) * g_nb_pipe)))
			printf("==================== MALLOC ERROR ======================\n");
		fill_pipe_fd(g_nb_pipe);
		i = -1;
		while (g_cmd_call[++i].cmd)
		{
			//printf("SEMICOLONS = %d | PIPE = %d\n", g_nb_semicolons, g_nb_pipe);
			printf("CMD = %s | PARAM = %s\n", g_cmd_call[i].cmd, g_cmd_call[i].param_line);
			continue;
			proc_called++;
			if (proc_called < g_nb_pipe)
				pipe(g_pipe_fd[proc_called]);
			j = -1;
			while (++j < 4)
			{
				if (!ft_strcmp(g_cmd_str[j], g_cmd_call[i].cmd))
				{
					g_cmd_fun[j](g_cmd_call[i].param_line, &envp);
					break;
				}
			}
			if (j != 4)
				continue;
			g_pid = fork();
			if (g_pid == 0)
			{
				/*
				** for Writing
				*/
				if (g_nb_pipe && g_cmd_call[i + 1].cmd)
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
				/*if (!i)
				{
					write(1, g_cmd_call[i].param_line, ft_strlen(g_cmd_call[i].param_line));
					//printf("FIRST CMD %s\n", "We Can");
					exit(1);
				}
				else
				*/
				execve(ft_strjoin(cur_dir, g_cmd_call[i].cmd), &g_cmd_call[i].param_line, envp);
				printf("minishell hhhh :( : ERROR\n");
				exit(-1);
			}
			else
			{
				wait(&status);
				if (proc_called < g_nb_pipe)
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
