NAME := ft_nm

CC = gcc 
CFLAGS = -Wall -Wextra -Werror
LFLAGS = -L./libft -lft
IFLAGS = -I./inc/ -I./libft/includes/

LIBFT = libft/libft.a

SRC_DIR = ./src/
INC_DIR = ./inc/
OBJ_DIR = ./obj/

SRC_FILES =	nm.c \
			swap.c

OBJ_FILES = $(SRC_FILES:.c=.o)
INC_FILES = nm.h \
			swap.h

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
INC = $(addprefix $(INC_DIR), $(INC_FILES))

.PHONY: all clean fclean re $(LIBFT) 

all: $(OBJ_DIR) $(LIBFT) $(NAME) auteur

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

norme: $(SRC)
	norminette $(SRC) $(INC)

$(NAME): $(OBJ)
	@$(CC) -o $@ $^ $(LFLAGS)

$(LIBFT):
	@make -C libft/

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC)
	@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<

clean:
	@make -C libft/ clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(LIBFT)

re: fclean all
