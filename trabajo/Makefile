SRCS=$(addsuffix .c, \
	00_main \
	01_rl \
	02_signals \
	03_env \
	10_init \
	20_tokenize \
	30_ast \
	40_exec_ast \
	41_builtins \
	50_utils \
	51_helpers \
	60_free_management \
	\
	)

#	\
	31_inlst \
	32_outlst \
	


all :
#	@make -C ./libft/ all
	@cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell \
	-L/usr/local/lib -lreadline -L./libft/ -lft

ess : essayage.c
	@cc -Wall -Werror -Wextra -I./libft/includes essayage.c 03_env.c 50_utils.c 51_helpers.c 60_free_management.c -o ess \
	-L/usr/local/lib -lreadline -L./libft/ -lft

mnsh1	:
	@make -C ./libft/ all
	@cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell1 \
	-L/usr/local/lib -lreadline -L./libft/ -lft

mnsh2	:
	@make -C ./libft/ all
	@cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell2 \
	-L/usr/local/lib -lreadline -L./libft/ -lft