# include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	int i;

	i = skip_spaces(*argv);
	if ((*argv)[i])
	{
		printf("minishell: too many arg !!\n");
		return (0);
	}
	printf("%s\n", ft_getcwd());
}
