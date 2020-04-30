# include "minishell.h"

char	*ft_getcwd(void)
{
	int	nb;
	char	*old_path;
	char	*path;

	nb = 50;
	path = "";
	old_path = "0";
	while (ft_strcmp(path, old_path))
	{
		old_path = path;
		path = (char*)malloc(nb);
		if (nb != 50)
			free(old_path);
		getcwd(path, nb);
		nb *= 2;
	}
	return (path);

}
