#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdlib.h>
# include "libft.h"
# include "GNL/get_next_line.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# define DELIM_SEMICOLON ft_split(";", ' ')
# define DELIM_PIPE ft_split("|", ' ')
# define DELIM_OUT_REDIR ft_split(">", ' ')
# define DELIM_IN_REDIR ft_split("<", ' ')
# define DELIM_APPEND_REDIR ft_split(">>", ' ')
# define ALL_DELIM ft_split("| >> < >", ' ')
# define ft_putstr(str) ft_putstr_fd(str, 1)
# define ft_putchar(c) ft_putchar_fd(c, 1);
# define ENVP_GROW 6

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
	char	is_semicolon;
	/*
	**----------------------------------------------**
	** cmd_pos is for position of the pipe
	**----------------------------------------------**
	*/
	int	cmd_pos;
}		t_cmd;

typedef struct	s_whole_cmd
{
	char	*cmd_line;
	t_cmd	*cmd_treated;
}		t_whole_cmd;

/*
**------------------------------------------------------**
** Here some global variables for some functions
**------------------------------------------------------**
*/

t_whole_cmd	*g_all_cmd;
int		g_nb_redirections;
int		g_nb_pipe;
int		g_nb_semicolons;
int		g_envp_count;
int		g_envp_last_index;

int		g_export_equal_index;

t_cmd		*g_cmd_call;
int		g_stdio_fd[2];
int		**g_pipe_fd;
pid_t		g_pid;

char	*g_cmd_str[5];
void	(*g_cmd_fun[4])(char *arg, char ***envp);


int	*find_quotes(char *arg);
void	error_and_exit(char *error);
void	remove_backslash(char *arg, int *pos);
char	*ft_getcwd(void);
char	*get_env(char *needle, char **env);
void	waiting_new_cmd(void);
void	shell_loop(char** envp);
void	init(char** envp);
int	ft_strcmp(const char *s1, const char *s2);
int     count_word(char *line);
int	skip_spaces(char *str);
void	ft_cd(char *arg, char ***envp);
void	ft_export(char *arg, char ***envp);
char	*env_var_concat(char *arg, int *i, char **envp);
void	ft_unset(char *arg, char ***envp);
char	*remove_quotes(char *param);
void	ft_exit(char *arg, char ***envp);

#endif
