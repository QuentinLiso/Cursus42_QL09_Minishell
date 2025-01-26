SRCS=$(addsuffix .c, \
	1 \
	2_quotes \
	main \
	\
	)


all :
	cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell \
	-L/usr/local/lib -lreadline -L./libft/ -lft