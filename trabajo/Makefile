SRCS=$(addsuffix .c, \
	ast \
	exec_ast \
	helpers \
	init \
	main \
	tokenize \
	utils \
	\
	)


all :
	@cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell \
	-L/usr/local/lib -lreadline -L./libft/ -lft

make ess : essayage.c
	@cc -Wall -Werror -Wextra -I./libft/includes essayage.c -o ess \
	-L/usr/local/lib -lreadline -L./libft/ -lft