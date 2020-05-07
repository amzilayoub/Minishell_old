# include "../minishell.h"

int	main(int argc, char**argv, char **envp)
{
	char	*file_name;
	char	*line;
	int	fd;
	int	n;


	line = (char*)malloc(100);
	// make some string manipulation before, such as:
	//	-remove backslashes
	//	-quotes
	//	-spaces after the file_name
	file_name = argv[0];
	//printf("%s\n", file_name);
//	if ((fd = open(file_name, O_TRUNC | O_WRONLY | O_APPEND)) < 0)
	if ((fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY | O_APPEND)) < 0)
	{
		printf("OPEN ERROR\n");
		return (0);
	}
	//fd = open(file_name, O_WRONLY | O_APPEND);
	(read(0, line, 100));
	//while ((n = read(0, line, 100)) > 0)
	{
		write(fd, line, ft_strlen(line));
		close(fd);
		close(0);
		write(1, line, ft_strlen(line));
	}
}
