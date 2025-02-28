# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/27 14:14:33 by nefadli           #+#    #+#              #
#    Updated: 2025/02/27 17:29:42 by nefadli          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

# ################ #
#*    COULEURS    *#
# ################ #

CLASSIC	= \e[0m
WHITE	= \e[38;2;238;232;213m
BOLD	= \e[1m
RED   	= \e[31m
GREEN  	= \e[32m
YELLOW 	= \e[38;2;250;189;0m
CYAN   	= \e[36m

# ################### #
#*    VARIABLES      *#
# ################### #

NAME        =   minishell

# Source files list
SRCS        =   $(addsuffix .c, \
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
)

# Object files generated in the .objs/ directory
OBJS_PATH   =   .objs/
OBJS        =   $(SRCS:%.c=$(OBJS_PATH)%.o)

# Include and libft paths
INC_PATH    =   include/
LIBFT_PATH  =   ./libft
CFLAGS += -I$(INC_PATH) -I$(LIBFT_PATH)/includes
LIBFT_LIB   =   $(LIBFT_PATH)/libft.a

# Compilation settings
CC          =   cc
CFLAGS      =   -Wall -Werror -Wextra -g3
CFLAGS      +=  -I/opt/homebrew/opt/readline/include
LFLAGS      =   -L$(LIBFT_PATH) -lft -lreadline
RM          =   rm -rf

# ################### #
#*    REGLES         *#
# ################### #

all: $(OBJS_PATH) $(LIBFT_LIB) $(NAME)

# Rule to create the object directory
$(OBJS_PATH):
	@mkdir -p $(OBJS_PATH)

# Rule to build the library only if it needs to be rebuilt
$(LIBFT_LIB):
	@make -C $(LIBFT_PATH) all

# Rule to compile object files
$(OBJS_PATH)%.o: %.c | $(OBJS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link and create the final executable
$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LFLAGS)
	@echo "$(BOLD)$(GREEN)[DONE] Minishell created!$(CLASSIC)"

ess: essayage.c
	@$(CC) $(CFLAGS) essayage.c 03_env.c 50_utils.c 51_helpers.c 60_free_management.c -o ess $(LFLAGS)
	@echo "$(BOLD)$(GREEN)[DONE] Ess created!$(CLASSIC)"

mnsh1:
	@$(CC) $(CFLAGS) $(SRCS) -o minishell1 $(LFLAGS)
	@echo "$(BOLD)$(GREEN)[DONE] Minishell1 created!$(CLASSIC)"

mnsh2:
	@$(CC) $(CFLAGS) $(SRCS) -o minishell2 $(LFLAGS)
	@echo "$(BOLD)$(GREEN)[DONE] Minishell2 created!$(CLASSIC)"

clean:
	$(RM) $(OBJS_PATH)
	@make clean -C $(LIBFT_PATH)
	@echo "$(BOLD)$(RED)[DONE] Object files deleted...$(CLASSIC)"

# Full cleanup
fclean: clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT_PATH)
	@echo "$(BOLD)$(RED)[DONE] Minishell deleted...$(CLASSIC)"

# Complete recompilation
re: fclean all
	@echo "$(BOLD)$(GREEN)[DONE] Minishell recompiled!$(CLASSIC)"

.PHONY: all clean fclean re ess mnsh1 mnsh2