# include <stdio.h>
# include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main ()
{
	int fd[2];
	int status;

	pipe(fd);
	if (!fork())
	{
		int stdoutfd = dup(1);
		dup2(fd[1], 1);
		dup2(fd[0], 0);
		char s[10];
		write(1, "HiIiIc\n", 6);
		read(0, s, 6);
		write(1, "TEST\n", 5);
		dup2(stdoutfd, 1);
		execve("prog2", NULL, NULL);
		exit(1);
	}
	else
	{
		wait(&status);
	}
}
