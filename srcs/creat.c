#include "../includes/cub.h"

int	close_window(t_data *data)
{
	free_all(data);
	exit(0);
	return (0);
}

int	key_handler(int keycode, t_data *data)
{
	if (keycode == 65307)
	{
		free_all(data);
		exit(0);
	}
	return (0);
}

void put_pixel(t_data *data, int x, int y, int color)
{
    char *dst;
    dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
    *(unsigned int*)dst = color;
}

void	creat_floor_celling(t_data *data)
{
	int x;
	int y;

	y = 0;
	while(y < WIN_H)
	{
		x = 0;
		while(x < WIN_W)
		{
			if (y < WIN_H / 2)
				put_pixel(data, x, y, data->c);
			else
				put_pixel(data, x, y, data->f);
			x++;
		}
		y++;
	}
}
void	init_game(t_data *data)
{
	data->mlx = mlx_init();
	if (data->mlx == NULL)
	{
		printf("Error\n:mlx_init\n");
		free_all(data);
		exit(1);
	}
	data->win = mlx_new_window(data->mlx, WIN_W, WIN_H, "CUB3D!");
	if (!data->win)
		window_error(data);
	data->img = mlx_new_image(data->mlx, WIN_W, WIN_H);
    if (!data->img)
    {
		perror("Error\n");
        mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		free_all(data);
        exit(EXIT_FAILURE);
    }
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_length, &data->endian);
	if (!data->img)
    {
		perror("Error\n");
		mlx_destroy_image(data->mlx, data->img);
        mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		free_all(data);
        exit(EXIT_FAILURE);
    }
}

void	creat_game(t_data *data)
{
	init_game(data);
	init_player_direction(data);
	creat_floor_celling(data);
	raycasting(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	mlx_hook(data->win, 17, 1L << 17, close_window, data);
	mlx_hook(data->win, 2, 1L << 0, key_handler, data);
	mlx_loop(data->mlx);
}
