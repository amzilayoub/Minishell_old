all:
	gcc *.c GNL/*.c builtins/cd.c builtins/ft_export.c builtins/ft_exit.c builtins/ft_unset.c builtins/get_env.c builtins/skip_spaces.c libft.a
	gcc builtins/redirect_write.c libft.a GNL/*.c -o "builtins/>"
run: all
	./a.out
