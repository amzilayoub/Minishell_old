# include "../minishell.h"
       #include <sys/types.h>
              #include <sys/stat.h>
                     #include <fcntl.h>
                     


int	main(int argc, char **argv, char**envp)
{
	char s[10];
	read(0, s, 50);
	printf("FROM ECHO %s | READ = %s\n", *argv, s);
}
