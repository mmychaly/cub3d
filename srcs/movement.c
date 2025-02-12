/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 03:09:12 by mmychaly          #+#    #+#             */
/*   Updated: 2025/02/12 06:27:33 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	else if (keycode == 65361)
	{
		rotate_left(data);
		draw_img(data);
	}
	else if (keycode == 65363)
	{
		rotate_right(data);
		draw_img(data);
	}
	else if (keycode == 119)//w
	{
		move_forward(data);
		draw_img(data);
	}
	else if (keycode == 115)//s
	{
		move_backward(data);
		draw_img(data);
	}
	else if (keycode == 100)//d
	{
		move_right(data);
		draw_img(data);
	}
	else if (keycode == 97)//a
	{
		move_left(data);
		draw_img(data);
	}
	return (0);
}

void	rotate_left(t_data *data)
{
	double old_dir_x;
    double old_plane_x;
    double speed;

	speed = -ROT_SPEED;
	old_dir_x = data->player.dir_x;
	old_plane_x  = data->player.plane_x;
	
	data->player.dir_x = data->player.dir_x * cos(speed) - data->player.dir_y * sin(speed);
	data->player.dir_y = old_dir_x * sin(speed) + data->player.dir_y * cos(speed);

	data->player.plane_x = data->player.plane_x * cos(speed) - data->player.plane_y * sin(speed);
	data->player.plane_y = old_plane_x * sin(speed) + data->player.plane_y * cos(speed);
}

void	rotate_right(t_data *data)
{
	double old_dir_x;
    double old_plane_x;
    double speed;

	speed = ROT_SPEED;
	old_dir_x = data->player.dir_x;
	old_plane_x  = data->player.plane_x;
	
    data->player.dir_x = data->player.dir_x * cos(speed) - data->player.dir_y * sin(speed);
    data->player.dir_y = old_dir_x * sin(speed) + data->player.dir_y * cos(speed);

    data->player.plane_x = data->player.plane_x * cos(speed) - data->player.plane_y * sin(speed);
    data->player.plane_y = old_plane_x * sin(speed) + data->player.plane_y * cos(speed);
}

void move_forward(t_data *data)
{
    double new_x;
    double new_y;
    
    new_x = data->player.pos_x + data->player.dir_x * MOVE_SPEED;
    new_y = data->player.pos_y + data->player.dir_y * MOVE_SPEED;
    
	//Проверка на стену
    if (data->map[(int)data->player.pos_y][(int)new_x] != '1')
        data->player.pos_x = new_x;
    if (data->map[(int)new_y][(int)data->player.pos_x] != '1')
        data->player.pos_y = new_y;
}

void move_backward(t_data *data)
{
    double new_x;
    double new_y;
    
    // Движение назад - то же самое, что вперед, но с отрицательной скоростью
    new_x = data->player.pos_x - data->player.dir_x * MOVE_SPEED;
    new_y = data->player.pos_y - data->player.dir_y * MOVE_SPEED;
    
    if (data->map[(int)data->player.pos_y][(int)new_x] != '1')
        data->player.pos_x = new_x;
    if (data->map[(int)new_y][(int)data->player.pos_x] != '1')
        data->player.pos_y = new_y;
}

void move_left(t_data *data)
{
    double new_x;
    double new_y;
    
    new_x = data->player.pos_x - data->player.plane_x * MOVE_SPEED;
    new_y = data->player.pos_y - data->player.plane_y * MOVE_SPEED;
    
    if (data->map[(int)data->player.pos_y][(int)new_x] != '1')
        data->player.pos_x = new_x;
    if (data->map[(int)new_y][(int)data->player.pos_x] != '1')
        data->player.pos_y = new_y;
}

void move_right(t_data *data)
{
    double new_x;
    double new_y;
    
    new_x = data->player.pos_x + data->player.plane_x * MOVE_SPEED;
    new_y = data->player.pos_y + data->player.plane_y * MOVE_SPEED;
    
    if (data->map[(int)data->player.pos_y][(int)new_x] != '1')
        data->player.pos_x = new_x;
    if (data->map[(int)new_y][(int)data->player.pos_x] != '1')
        data->player.pos_y = new_y;
}

int handle_mouse(int x, int y, t_data *data)
{
    static int last_x = -1; ;
    int delta_x;
    double old_dir_x;
    double old_plane_x;
//	int speed;
	double sensitivity = 0.006;

//	speed = ROT_SPEED;
    (void)y;// Игнорируем Y координату мыши
    if (last_x == -1)
    {
        last_x = x;
        return (0);
    }
    

    delta_x = x - last_x;
    last_x = x;
    
    if (delta_x == 0)//Выходим если не было движения мышью, хотя по идеи тогда hook не должен был сработать 
        return (0);
        
    old_dir_x = data->player.dir_x;
    old_plane_x = data->player.plane_x;
    
    // Поворачиваем вектор направления и плоскость камеры
    // Используем delta_x для определения направления поворота
    double rotation = delta_x * sensitivity;
    
    // Поворот вектора направления
    data->player.dir_x = data->player.dir_x * cos(rotation) - data->player.dir_y * sin(rotation);
    data->player.dir_y = old_dir_x * sin(rotation) + data->player.dir_y * cos(rotation);
    
    // Поворот плоскости камеры
    data->player.plane_x = data->player.plane_x * cos(rotation) - data->player.plane_y * sin(rotation);
    data->player.plane_y = old_plane_x * sin(rotation) + data->player.plane_y * cos(rotation);
//	draw_img(data);
    return (0);
}