NAME = cub3D
SRC_F = srcs/free_error.c\
		srcs/main.c \
		srcs/parser_utils.c \
		srcs/creat.c \
		srcs/raycasting.c
CC = cc
FLAGS = -Wall -Werror -Wextra
LIB = -Lminilibx-linux -lmlx_Linux -Llibft -lft -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz
RM = rm -f

$(NAME): $(SRC_F)
	@make all -C libft
	$(CC) $(FLAGS) $^ -o $@ $(LIB) -Iincludes

all: $(NAME)

clean:
	make clean -C libft

fclean: clean
	make fclean -C libft
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re