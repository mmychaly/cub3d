#ifndef CUB_H
#define CUB_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <math.h>

# define WIN_W  800
# define WIN_H  600
# define TEX_SIZE   64
# define ROT_SPEED 0.05
# define MOVE_SPEED 0.05

typedef struct s_player
{
    double		pos_x;
	double		pos_y;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
	char    direction;
}			t_player;

typedef struct s_img_data
{
    void *img;
    char *addr;
    int bpp;
    int line_length;
    int endian;
}           t_img_data;

typedef struct s_data
{
    char    *no;
    char    *so;
    char    *ea;
    char    *we;

    int     f;
    int     c;
    t_img_data north;
    t_img_data south;
    t_img_data west;
    t_img_data east;
    t_img_data image;

    char    **map;
    int     map_height;
    int     map_width;
    void *mlx;
    void *win;
    t_player	player;
}           t_data;

int	ft_atoi(const char *nptr);
void	*ft_memset(void *s, int c, size_t n);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
char	*get_next_line(int fd);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

void	creat_game(t_data *data);
void	init_game(t_data *data);
int     draw_img(t_data *data);
void	creat_floor_celling(t_data *data);
void put_pixel(t_data *data, int x, int y, int color);


int	key_handler(int keycode, t_data *data);
int	close_window(t_data *data);
void	rotate_left(t_data *data);
void	rotate_right(t_data *data);
void    move_forward(t_data *data);
void    move_backward(t_data *data);
void    move_left(t_data *data);
void    move_right(t_data *data);
int handle_mouse(int x, int y, t_data *data);

void	free_strs(char **strs);
void	free_all(t_data *data);
void	window_error(t_data *data);

void printf_map(t_data *data);//Удалить перед сдачей
void remove_newline(char *str);
int	define_colors(char *str);

int	check_first_line(char *str);
int get_map_dimensions(t_data *data);
void	init_map(t_data *data,char *str,int fd,char **temp);
void	init_data(t_data *data, char **argv);


void init_player_direction(t_data *data);
void draw_vertical_line(t_data *data, int x, int draw_start, int draw_end, int color);
void raycasting(t_data *data);

void loading_texture(t_data *data, t_img_data *texture, char *path);
unsigned int get_texture_color(t_img_data *texture, int x, int y);
void draw_textured_line(t_data *data, int x, int draw_start, int draw_end, t_img_data *texture, double wall_x, int line_height);
#endif