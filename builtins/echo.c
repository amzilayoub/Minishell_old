# include "../minishell.h"
       #include <sys/types.h>
              #include <sys/stat.h>
                     #include <fcntl.h>
                     


int	main(int argc, char **argv, char**envp)
{
	char *s;
	get_next_line(0, &s);
	printf("READ = %s | ARG = %s\n", s, *argv);
}
