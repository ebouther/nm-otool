FT_NM := ft_nm
FT_OTOOL := ft_otool

CC = gcc 
CFLAGS = -Wall -Wextra -Werror 			-g

LIBFT_PATH = ./libft/
LIBFT = $(addprefix $(LIBFT_PATH),libft.a)

LFLAGS = -L$(LIBFT_PATH) -lft
IFLAGS = -I./inc/ -I $(addprefix $(LIBFT_PATH), includes/)


SRC_DIR = ./src/
INC_DIR = ./inc/
OBJ_DIR = ./obj/

SRC_FILES =	ar.c \
			fat.c \
			macho.c \
			nm_otool.c \
			symbols.c \
			swap.c \
			swap_int.c \
			utils.c


OBJ_FILES = $(SRC_FILES:.c=.o)

INC_FILES = masks.h \
			nm_otool.h \
			symbols.h \
			swap.h \
			utils.h \
			nm.h

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
INC = $(addprefix $(INC_DIR), $(INC_FILES))

# ---------- NM ----------- #
NM_SRC_FILES = nm.c
NM_SRC = $(addprefix $(SRC_DIR), $(NM_SRC_FILES))

NM_OBJ_FILES =  $(NM_SRC_FILES:.c=.o)
NM_OBJ = $(addprefix $(OBJ_DIR), $(NM_OBJ_FILES))
# _________________________ #


# --------- OTOOL --------- #
OTOOL_SRC_FILES = otool.c
OTOOL_SRC = $(addprefix $(SRC_DIR), $(OTOOL_SRC_FILES))

OTOOL_OBJ_FILES =  $(OTOOL_SRC_FILES:.c=.o)
OTOOL_OBJ = $(addprefix $(OBJ_DIR), $(OTOOL_OBJ_FILES))
# _________________________ #




.PHONY: all clean fclean re $(LIBFT) norme

all: $(OBJ_DIR) $(LIBFT) $(FT_NM) $(FT_OTOOL)


$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

norme: $(SRC)
	norminette $(SRC) $(INC)

$(FT_NM): $(OBJ) $(NM_OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(FT_OTOOL): $(OBJ) $(OTOOL_OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(LIBFT):
	make -C $(LIBFT_PATH) 

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC)
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<

clean:
	make -C $(LIBFT_PATH) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(FT_NM) $(FT_OTOOL) $(LIBFT)

re: fclean all
