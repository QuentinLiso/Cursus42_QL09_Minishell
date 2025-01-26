# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qliso <qliso@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/09 10:54:38 by qliso             #+#    #+#              #
#    Updated: 2024/11/09 10:54:43 by qliso            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source files .c
LIBFT_FOLD = ./ft_libft/
FTPRINTF_FOLD = ./ft_printf/
FTGNL_FOLD = ./ft_getnextline/
SRC = $(addsuffix .c, \
		$(addprefix $(LIBFT_FOLD), \
		ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint ft_strlen \
		ft_memset ft_bzero ft_memcpy ft_memmove ft_strlcpy ft_strlcat ft_toupper ft_tolower \
		ft_strchr ft_strrchr ft_strncmp ft_memchr ft_memcmp ft_strnstr ft_atoi ft_calloc \
		ft_strdup ft_substr ft_strjoin ft_strtrim ft_split ft_itoa ft_strmapi ft_striteri \
		ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd \
		ft_lstnew ft_lstadd_front ft_lstsize ft_lstlast ft_lstadd_back ft_lstdelone \
		ft_lstclear ft_lstiter ft_lstmap) \
		$(addprefix $(FTPRINTF_FOLD), \
		ft_print_char ft_print_hexnbr ft_print_nbr ft_print_ptr ft_print_str ft_print_unbr \
		ft_printf) \
		$(addprefix $(FTGNL_FOLD), \
		get_next_line get_next_line_utils) \
		)


# Objects files created from sources files .o
OBJS := $(SRC:%.c=%.o)

# Includes folder and header files
INCLUDES = ./includes/
HEADERS = $(addsuffix .h, \
			$(addprefix $(INCLUDES), libft ft_printf get_next_line) \
			)

# Libname
NAME = libft.a

# Preprocessing flags
CPPFLAGS = -I$(INCLUDES)

# Compile flags
CCFLAGS = -Wall -Wextra -Werror

# Buffer size for printf
BUFFER_SIZE = 1024

# Rules
.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
		ar rcs $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
		$(CC) $(CPPFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) $(CCFLAGS) -o $@ -c $<

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

testmain: main.c
		$(CC) $(CCFLAGS) $(CPPFLAGS) -L. main.c -l:$(NAME) -o testmain

