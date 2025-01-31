SRCS=$(addsuffix .c, \
	ast \
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