# include <stdio.h>
# include <stdio.h>
# include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>



int main()
{
	char s[10];
	read(0, s, 10);
	printf("%s\n", s);
}
