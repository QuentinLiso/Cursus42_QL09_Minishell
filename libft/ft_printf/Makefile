# Contient les fichiers sources .c pour les bonus
SRC = $(addprefix ./srcs/, $(addsuffix .c, ft_print_char ft_print_hexnbr ft_print_nbr ft_print_ptr \
		ft_print_str ft_print_unbr ft_printf))

# Liste des fichiers objets créés à partir des fichier sources
OBJS := $(SRC:%.c=%.o)

# Variable contenant le nom de la librairie
NAME = libftprintf.a

# Variable contenant le nom du compilateur
CC = gcc

# Flags pour la compilation
CCFLAGS = -Wall -Wextra -Werror

# Dossier includes
INC_DIR = ./include

# Variable contenant les flags de préprocesseur
CPPFLAGS = -I$(INC_DIR)

.PHONY: all clean fclean re

all: $(NAME) 

$(NAME): $(OBJS)
		ar rcs $(NAME) $(OBJS)

%.o: %.c
		$(CC) $(CPPFLAGS) $(CCFLAGS) -o $@ -c $<

clean:
		rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all
