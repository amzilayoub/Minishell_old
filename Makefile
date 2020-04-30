all:
	gcc *.c GNL/*.c builtins/cd.c builtins/ft_export.c builtins/ft_exit.c builtins/ft_unset.c builtins/get_env.c builtins/skip_spaces.c libft.a
run: all
	./a.out
