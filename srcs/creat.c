#include "../includes/cub.h"

void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->image.addr + (y * data->image.line_length + x * (data->image.bpp
				/ 8));
	*(unsigned int *)dst = color;
}

unsigned int	get_texture_color(t_img_data *texture, int x, int y)
{
	char	*dst;

	dst = texture->addr + (y * texture->line_length + x * (texture->bpp / 8));
	return (*(unsigned int *)dst);
}

void	creat_floor_celling(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
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
		error_with_exit(data, NULL, NULL, NULL);
	data->image.img = mlx_new_image(data->mlx, WIN_W, WIN_H);
	if (!data->image.img)
	{
		perror("Error\n");
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		free_all(data);
		exit(EXIT_FAILURE);
	}
	data->image.addr = mlx_get_data_addr(data->image.img, &data->image.bpp,
			&data->image.line_length, &data->image.endian);
	if (!data->image.addr)
	{
		perror("Error\n");
		mlx_destroy_image(data->mlx, data->image.img);
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		free_all(data);
		exit(EXIT_FAILURE);
	}
	loading_texture(data, &data->north, data->no);
	loading_texture(data, &data->south, data->so);
	loading_texture(data, &data->east, data->ea);
	loading_texture(data, &data->west, data->we);
}

void	loading_texture(t_data *data, t_img_data *texture, char *path)
{
	int	height;
	int	width;

	height = TEX_SIZE;
	width = TEX_SIZE;
	texture->img = mlx_xpm_file_to_image(data->mlx, path, &width, &height);
	if (!texture->img)
	{
		printf("Error\nFailed to load texture: %s\n", path);
		free_all(data); //Проработать выход
		exit(1);
	}
	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp,
			&texture->line_length, &texture->endian);
	if (!texture->addr)
	{
		printf("Error\nFailed to get texture data: %s\n", path);
		mlx_destroy_image(data->mlx, texture->img);
		free_all(data); //Проработать выход
		exit(1);
	}
}

int	draw_img(t_data *data)
{
	creat_floor_celling(data);
	raycasting(data);
	mlx_put_image_to_window(data->mlx, data->win, data->image.img, 0, 0);
	return (0);
}

void	creat_game(t_data *data)
{
	init_game(data);
	init_player_direction(data);
	draw_img(data);
	mlx_hook(data->win, 17, 1L << 17, close_window, data);
	mlx_hook(data->win, 2, 1L << 0, key_handler, data);
	mlx_hook(data->win, 6, 1L << 6, handle_mouse, data);
	//	mlx_mouse_hide(data->mlx, data->win);
	mlx_mouse_move(data->mlx, data->win, WIN_W / 2, WIN_H / 2);
	mlx_loop_hook(data->mlx, draw_img, data);
	mlx_loop(data->mlx);
}

/*
void	loading_textures(t_data *data)
{
	int	height;
	int	width;

	height = TEX_SIZE;
	width = TEX_SIZE;
	data->north.img = mlx_xpm_file_to_image(data->mlx,
			data->no, &height, &width);
	data->north.addr = mlx_get_data_addr(data->north.img, &data->north.bpp,
			&data->north.line_length, &data->north.endian);
	data->south.img = mlx_xpm_file_to_image(data->mlx,
			data->so, &height, &width);
	data->south.addr = mlx_get_data_addr(data->south.img, &data->south.bpp,
			&data->south.line_length, &data->south.endian);
	data->west.img = mlx_xpm_file_to_image(data->mlx,
			data->we, &height, &width);
	data->west.addr = mlx_get_data_addr(data->west.img, &data->west.bpp,
			&data->west.line_length, &data->west.endian);
	data->east.img = mlx_xpm_file_to_image(data->mlx,
			data->ea, &height, &width);
	data->east.addr = mlx_get_data_addr(data->east.img, &data->east.bpp,
			&data->east.line_length, &data->east.endian);
	if (!data->north.img || !data->south.img || !data->west.img
		|| !data->east.img)
	{
		write(2, "Error\n: image is not loaded\n", 28);
		free_all(data);//Проработать ,освободить подгурзившиеся изображения
		exit(EXIT_FAILURE);
	}
}
*/