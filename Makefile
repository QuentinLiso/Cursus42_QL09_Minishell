# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/27 14:14:33 by nefadli           #+#    #+#              #
#    Updated: 2025/03/01 17:21:47 by nefadli          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
SRCS_PATH	=	./srcs/
SRCS_FILES	=  $(addsuffix .c, \
		00_main \
		01_init \
		02_env \
		02_env2 \
		03_signals \
		10_loop \
		20_tokenize \
		20_tokenize_2 \
		20_tokenize_3 \
		20_tokenize_4 \
		20_tokenize_5 \
		30_ast \
		30_ast_1 \
		30_ast_2 \
		30_ast_3 \
		30_ast_4 \
		40_exec_ast \
		40_exec_ast_1 \
		40_exec_ast_2 \
		40_exec_ast_3 \
		40_exec_ast_4 \
		40_exec_ast_5\
		50_builtins \
		50_builtins_1 \
		50_builtins_2 \
		50_builtins_3 \
		60_free_management \
		60_free_management_1 \
		70_utils \
		70_utils_1 \
		70_utils_2 \
		71_helpers \
		71_helpers_1 \
	)
SRCS	= $(addprefix $(SRCS_PATH), $(SRCS_FILES))

# Object files generated in the .objs/ directory
OBJS_PATH   =   .objs/
OBJS        =   $(SRCS_FILES:%.c=$(OBJS_PATH)%.o)

# Include and libft paths
INC_PATH    =   ./include/
LIBFT_PATH  =   ./libft
LIBFT_LIB   =   $(LIBFT_PATH)/libft.a

# Compilation settings
CC          =   cc
CFLAGS      =  -Wall -Werror -Wextra -g3
CFLAGS      +=  -I/opt/homebrew/opt/readline/include
CFLAGS 		+= 	-I$(INC_PATH) -I$(LIBFT_PATH)/includes
LFLAGS      =   -L$(LIBFT_PATH) -lft -lreadline
RM          =   rm -rf

# ################### #
#*    REGLES         *#
# ################### #

all: $(OBJS_PATH) $(LIBFT_LIB) $(NAME)

run:
	./minishell
runv:
	valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

# Rule to create the object directory
$(OBJS_PATH):
	@mkdir -p $(OBJS_PATH)

# Rule to build the library only if it needs to be rebuilt
$(LIBFT_LIB):
	@make -C $(LIBFT_PATH) all

# Rule to compile object files
$(OBJS_PATH)%.o: $(SRCS_PATH)%.c | $(OBJS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link and create the final executable
$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LFLAGS)
	@echo "$(BOLD)$(GREEN)[DONE] Minishell created!$(CLASSIC)"

ess: essayage.c
	@$(CC) $(CFLAGS) essayage.c -o ess $(LFLAGS)
	@echo "$(BOLD)$(GREEN)[DONE] Ess created!$(CLASSIC)"

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
