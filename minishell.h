#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdlib.h>
# include "libft.h"
# include "GNL/get_next_line.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>

/*
**--------------------------------*
**struct of commands with params
**--------------------------------*
*/

typedef	struct	s_cmd
{
	char	*cmd;
	char	*param_line;
	char	*param_array;
	/*
	**----------------------------------------------**
	** cmd_pos is for position of the pipe
	**----------------------------------------------**
	*/
	int	cmd_pos;
}		t_cmd;

t_cmd		*g_cmd_call;
int		g_stdio_fd[2];
int		g_pipe_fd[2];
pid_t		g_pid;

char	*g_cmd_str[8];



void	waiting_new_cmd(void);
void	shell_loop(char** envp);
void	init_cmd_str(void);
int	ft_strcmp(const char *s1, const char *s2);
int     count_word(char *line);

#endif
