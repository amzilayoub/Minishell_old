# include "minishell.h"

int	calc_nb_char(char *line, char **delim)
{
	int i;
	int count;
	char quote;
	int	j;

	quote = 0;
	i = -1;
	count = 0;
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
		j = -1;
		while (delim[++j])
		{
			if (!ft_strncmp(line + i, delim[j], ft_strlen(delim[j])) && !quote)
			{
				count ++;
				if (!ft_strncmp(delim[j], ">>", 2))
					i++;
				break;
			}
		}
	}
	return (count);
}

int	*get_pos_char(char *line, char **delim, int nb_char)
{
	int *pos;
	int i;
	int quote;
	int j;
	int k;


	i = -1;
	j = -1;
	quote = 0;
	pos = (int*)malloc(sizeof(int) * nb_char);
	while (line[++i])
	{
		if (line[i] == '\\')
		{
			i++;
			continue;
		}
		else if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if ((line[i] == '\'' || line[i] == '"') && quote == line[i])
			quote = 0;
		k = -1;
		while (!quote && delim[++k])
		{
			if (!quote && !ft_strncmp(line + i, delim[k], ft_strlen(delim[k])))
			{
				pos[++j] = i;
				if (!ft_strncmp(delim[k], ">>", 2))
					i++;
				break;
			}
		}
	}
	return (pos);
}
/*
void    cmd_position(char *line, int index)
{
        int i;
        int quote;
        int j;

        quote = 0;
        i = -1;
        g_all_cmd[index].cmd_treated[j = 0].cmd_pos = 0;
        while (line[++i])
        {
                /*
                **--------------------------------------------**
                **  check if there's the open quote or double quotes
                **--------------------------------------------**
                */
                /*
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
                		g_all_cmd[index].cmd_treated[++j].cmd_pos = i;
                		i += (!ft_strncmp(line + i, ">>", 2)) ? 1 : 0;
                	}
                	else
                		g_all_cmd[index].cmd_treated[++j].cmd_pos = i + 1;
                	g_all_cmd[index].cmd_treated[j].is_semicolon = (line[i] == ';');
                }
        }
        g_all_cmd[index].cmd_treated[++j].cmd = 0;
}
*/

int	count_word(char *line)
{
	int i;

	i = 0;
	//printf("LINE = %s\n", line);
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

int	get_param_line(char *line, int start, t_cmd *next_cmd)
{
	int len;
	int next_pipe;

	//printf("NEXT POS %d\n", next_cmd->cmd_pos);
	next_pipe = (next_cmd->cmd_pos >= 0) ? next_cmd->cmd_pos - 1: ft_strlen(line);
	//printf("NEXT_PIPE = %d\n", next_cmd->cmd_pos);
	len = next_pipe - start;
	return (len <= 0 ? 0 : len);
}

void	init_cmd_array(int index)
{
	int i;

	i = -1;
	while (++i < g_nb_pipe + 2)
	{
		g_all_cmd[index].cmd_treated[i].cmd = 0;
		g_all_cmd[index].cmd_treated[i].param_line = 0;
		g_all_cmd[index].cmd_treated[i].param_array = 0;
		g_all_cmd[index].cmd_treated[i].cmd_pos = -1;
		g_all_cmd[index].cmd_treated[i].is_semicolon = 0;
	}
}

void	fit_cmdpos(int *pos, int index)
{
	int i;
	char c;

	i = -1;
	g_all_cmd[index].cmd_treated[0].cmd_pos = 0;
	while (++i < g_nb_pipe)
	{
		c = g_all_cmd[index].cmd_line[pos[i]];
		if (c == '>' || c == '<')
			g_all_cmd[index].cmd_treated[i + 1].cmd_pos = pos[i];
		else
			g_all_cmd[index].cmd_treated[i + 1].cmd_pos = pos[i] + 1;
	}
	g_all_cmd[index].cmd_treated[i].cmd = NULL;
}

void	prepare_line(int nb_pipe, int index)
{
	int i;
	int j;
	int len;
	int *pos;

	i = -1;
	//c = "|";
	/*
	**----------------------------------------------------**
	** add 1 for the first command
	**----------------------------------------------------**
	*/
	nb_pipe += 1;
	g_all_cmd[index].cmd_treated = (t_cmd*)malloc(sizeof(t_cmd) * (nb_pipe + 1));
	init_cmd_array(index);
	pos = get_pos_char(g_all_cmd[index].cmd_line, ALL_DELIM, nb_pipe);
	fit_cmdpos(pos, index);
	//cmd_position(g_all_cmd[index].cmd_line, index);
	//printf("SEM = %d | PIPE = %d\n", g_nb_semicolons, g_nb_pipe);
	while (++i < nb_pipe)
	{
		j = g_all_cmd[index].cmd_treated[i].cmd_pos;
		//printf("POS = %d\n", j);
		j += skip_spaces(g_all_cmd[index].cmd_line + j);
		//printf("NUM = %d\n", j);
		//while (line[++j])
		{
			//if (g_cmd_call[i + 1].cmd && i >= g_cmd_call[i + 1].cmd_pos)
				//break;
			//if (g_all_cmd[index].cmd_line[j] != '|')
			{
				len = count_word(g_all_cmd[index].cmd_line + j);
				g_all_cmd[index].cmd_treated[i].cmd = ft_substr(g_all_cmd[index].cmd_line, j, len);
				j += len + 1;
				len = get_param_line(g_all_cmd[index].cmd_line, j, &g_all_cmd[index].cmd_treated[i + 1]);
				//printf("LEN = %d\n", len);
				g_all_cmd[index].cmd_treated[i].param_line = ft_substr(g_all_cmd[index].cmd_line, j, len);
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

void	alloc_for_command(char *line)
{
	int i;
	int *pos;

	i = -1;
	//c = ";";
	g_nb_semicolons = calc_nb_char(line, DELIM_SEMICOLON) + 1;
	g_all_cmd = (t_whole_cmd*)malloc(sizeof(t_whole_cmd) * (g_nb_semicolons + 1));
	g_all_cmd[g_nb_semicolons].cmd_line = NULL;
	// i need to init here the g_all_cmd attr
	pos = get_pos_char(line, DELIM_SEMICOLON, g_nb_semicolons);
	if (g_nb_semicolons == 1)
		pos[0] = ft_strlen(line);
	while (++i < g_nb_semicolons)
	{
		//printf("POS %i = %i\n", i, pos[i]);
		if (i == 0)
			g_all_cmd[i].cmd_line = ft_substr(line, 0, pos[0]);
		else if (i + 1 == g_nb_semicolons)
			g_all_cmd[i].cmd_line = ft_substr(line, pos[i - 1] + 1, ft_strlen(line));
		else
			g_all_cmd[i].cmd_line = ft_substr(line, pos[i - 1] + 1, pos[i] - pos[i - 1] - 1);
	}
}

void	shell_loop(char **envp)
{
	char	*line;
	int	i;
	int	j;
	int	k;
	int	status;
	int	proc_called;
	char	*cur_dir;

	cur_dir = ft_strjoin(ft_getcwd(), "/builtins/");
	proc_called = -1;
	waiting_new_cmd();
	while (get_next_line(0, &line) > 0)
	{
		proc_called = -1;
		alloc_for_command(line);
		k = -1;
		while (g_all_cmd[++k].cmd_line)
		{
			g_nb_pipe = calc_nb_char(g_all_cmd[k].cmd_line, ALL_DELIM);
			//pipe_treatement(g_all_cmd[k].cmd_line);
			prepare_line(g_nb_pipe, k);
			if (!(g_pipe_fd = (int**)malloc(sizeof(int*) * g_nb_pipe)))
				printf("==================== MALLOC ERROR ======================\n");
			fill_pipe_fd(g_nb_pipe);
			//printf("PIPE = %d\n", g_nb_pipe);
			i = -1;
			g_cmd_call = g_all_cmd[k].cmd_treated;
			while (g_cmd_call[++i].cmd)
			{
				//printf("SEMICOLONS = %d | PIPE = %d\n", g_nb_semicolons, g_nb_pipe);
				//printf("%i- CMD = %s | PARAM = %s\n",k, g_cmd_call[i].cmd, g_cmd_call[i].param_line);
				//continue;
				proc_called++;
				if (proc_called < g_nb_pipe)
					pipe(g_pipe_fd[proc_called]);
					/*
					** for Writing
					*/
				if (g_nb_pipe && g_cmd_call[i + 1].cmd)
				{
					//printf("IF %d\n\n", i + 1);
					//close(1);
					//close(g_pipe_fd[proc_called][0]);
					dup2(g_pipe_fd[proc_called][1], 1);
					close(g_pipe_fd[proc_called][1]);
				}
				// second condition is for multi redirection ex: echo hhhh > f1 > f2
				else if (!g_cmd_call[i + 1].cmd)
				{
					//printf("ELSE IF %d\n\n", i + 1);
					//dup2(7, 1);
					int c = g_cmd_call[i].cmd[0];
					if (!(c == '<' || c == '>'))
						dup2(g_stdio_fd[1], 1);
					else
						close(1);
				}
				/*
				** for reading
				*/
				//printf("i = %i\n", i);
				if (i != 0)
				{
					//printf("IF i != 0\n", i + 1);
					//close(1);
					//printf("PROC_CALLED = %d\n", proc_called - 1);
					dup2(g_pipe_fd[proc_called - 1][0], 0);
					close(g_pipe_fd[proc_called - 1][0]);
				}
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

		}
		//close(g_pipe_fd[0]);
		dup2(g_stdio_fd[0], 0);
		dup2(g_stdio_fd[1], 1);
		waiting_new_cmd();
	}
}
