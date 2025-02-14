SRCS=$(addsuffix .c, \
	0_main \
	01_rl \
	02_signals \
	10_init \
	20_tokenize \
	30_ast \
	31_inlst \
	32_outlst \
	40_exec_ast \
	41_builtins \
	50_utils \
	51_helpers \
	60_free_management \
	\
	)


all :
	@cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell \
	-L/usr/local/lib -lreadline -L./libft/ -lft

make ess : essayage.c
	@cc -Wall -Werror -Wextra -I./libft/includes essayage.c 50_utils.c 51_helpers.c 60_free_management.c -o ess \
	-L/usr/local/lib -lreadline -L./libft/ -lft