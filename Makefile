NAME = cub3D
SRC_F = srcs/free_error.c\
		srcs/main.c \
		srcs/parser_utils.c \
		srcs/parser.c \
		srcs/parser_textures.c \
		srcs/parser_textures_utils.c \
		srcs/parser_map.c \
		srcs/parser_flood.c \
		srcs/parser_map_utils.c \
		srcs/creat.c \
		srcs/movement.c \
		srcs/raycasting.c \
		srcs/init.c
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