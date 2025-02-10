SRCS=$(addsuffix .c, \
	ast \
	builtins \
	exec_ast \
	expand \
	helpers \
	init \
	main \
	inlst \
	outlst \
	rl \
	tokenize \
	utils \
	\
	)


all :
	@cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell \
	-L/usr/local/lib -lreadline -L./libft/ -lft

make ess : essayage.c
	@cc -Wall -Werror -Wextra -I./libft/includes essayage.c utils.c helpers.c -o ess \
	-L/usr/local/lib -lreadline -L./libft/ -lft