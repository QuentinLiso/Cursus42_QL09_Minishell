SRCS=$(addsuffix .c, \
	30_ast \
	41_builtins \
	40_exec_ast \
	60_free_management \
	51_helpers \
	10_init \
	0_main \
	31_inlst \
	32_outlst \
	01_rl \
	02_signals \
	20_tokenize \
	50_utils \
	\
	)


all :
	@cc -Wall -Werror -Wextra -I./libft/includes $(SRCS) -o minishell \
	-L/usr/local/lib -lreadline -L./libft/ -lft

make ess : essayage.c
	@cc -Wall -Werror -Wextra -I./libft/includes essayage.c 50_utils.c 51_helpers.c -o ess \
	-L/usr/local/lib -lreadline -L./libft/ -lft