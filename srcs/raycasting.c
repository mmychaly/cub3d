#include "../includes/cub.h"

void init_player_direction(t_data *data)
{
    // Инициализация начального направления и плоскости камеры
    if (data->player.direction == 'N')
    {
        data->player.dir_x = -1.0;
        data->player.dir_y = 0.0;
        data->player.plane_x = 0.0;
        data->player.plane_y = 0.66;
    }
    else if (data->player.direction == 'S')
    {
        data->player.dir_x = 1.0;
        data->player.dir_y = 0.0;
        data->player.plane_x = 0.0;
        data->player.plane_y = -0.66;
    }
    else if (data->player.direction == 'E')
    {
        data->player.dir_x = 0.0;
        data->player.dir_y = 1.0;
        data->player.plane_x = 0.66;
        data->player.plane_y = 0.0;
    }
    else if (data->player.direction == 'W')
    {
        data->player.dir_x = 0.0;
        data->player.dir_y = -1.0;
        data->player.plane_x = -0.66;
        data->player.plane_y = 0.0;
    }
}

void draw_vertical_line(t_data *data, int x, int draw_start, int draw_end, int color)
{
    int y = draw_start;
    while (y < draw_end)
    {
        put_pixel(data, x, y, color);
        y++;
    }
}
void raycasting(t_data *data)
{
    int x = 0;
    while (x < WIN_W)
    {
        // 1. Вычисление позиции и направления луча
        double camera_x = 2 * x / (double)WIN_W - 1;
        double ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
        double ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;

        // 2. Позиция на карте
        int map_x = (int)data->player.pos_x;
        int map_y = (int)data->player.pos_y;

        // 3. Длина луча от текущей позиции до следующей стороны x или y
        double delta_dist_x = fabs(1 / ray_dir_x);
        double delta_dist_y = fabs(1 / ray_dir_y);

        // 4. Расстояние до следующей стороны x или y
        double side_dist_x;
        double side_dist_y;
        
        // 5. Направление шага
        int step_x;
        int step_y;
        
        // Определяем направление шага и начальное расстояние
        if (ray_dir_x < 0)
        {
            step_x = -1;
            side_dist_x = (data->player.pos_x - map_x) * delta_dist_x;
        }
        else
        {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - data->player.pos_x) * delta_dist_x;
        }
        if (ray_dir_y < 0)
        {
            step_y = -1;
            side_dist_y = (data->player.pos_y - map_y) * delta_dist_y;
        }
        else
        {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - data->player.pos_y) * delta_dist_y;
        }

        // 6. DDA алгоритм
        int hit = 0;
        int side;
        while (hit == 0)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }
            if (data->map[map_x][map_y] == '1')
                hit = 1;
        }

        // 7. Вычисление расстояния до стены
        double perp_wall_dist;
        if (side == 0)
            perp_wall_dist = (side_dist_x - delta_dist_x);
        else
            perp_wall_dist = (side_dist_y - delta_dist_y);

        // 8. Вычисление высоты линии для отрисовки
        int line_height = (int)(WIN_H / perp_wall_dist);
        int draw_start = -line_height / 2 + WIN_H / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = line_height / 2 + WIN_H / 2;
        if (draw_end >= WIN_H)
            draw_end = WIN_H - 1;

        // 9. Выбор цвета стены
        int color = 0xFF0000;  // Красный цвет для начала
        if (side == 1)
            color = color / 2;  // Делаем темнее для стен по Y

        // 10. Отрисовка вертикальной линии
        draw_vertical_line(data, x, draw_start, draw_end, color);
        x++;
    }
}
/*
void raycasting(t_data *data)
{
    int x = 0;
    while (x < WIN_W)
    {
        double camera_x = 2 * x / (double)WIN_W - 1;
        double ray_dir_x = data->player.dir_x + data->player.plane_x * camera_x;
        double ray_dir_y = data->player.dir_y + data->player.plane_y * camera_x;

        int map_x = (int)data->player.pos_x;
        int map_y = (int)data->player.pos_y;

        // Защита от деления на ноль
        double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
        double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);

        double side_dist_x;
        double side_dist_y;
        int step_x;
        int step_y;
        
        if (ray_dir_x < 0)
        {
            step_x = -1;
            side_dist_x = (data->player.pos_x - map_x) * delta_dist_x;
        }
        else
        {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - data->player.pos_x) * delta_dist_x;
        }
        if (ray_dir_y < 0)
        {
            step_y = -1;
            side_dist_y = (data->player.pos_y - map_y) * delta_dist_y;
        }
        else
        {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - data->player.pos_y) * delta_dist_y;
        }

        int hit = 0;
        int side;
        
        // Добавляем проверку границ карты
        while (hit == 0)
        {
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            }
            else
            {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }
            
            // Проверка границ карты
            if (map_x < 0 || map_x >= data->map_height || 
                map_y < 0 || map_y >= data->map_width)
                break;
                
            if (data->map[map_x][map_y] == '1')
                hit = 1;
        }

        if (hit)  // Рисуем стену только если попали в нее
        {
            double perp_wall_dist;
            if (side == 0)
                perp_wall_dist = side_dist_x - delta_dist_x;
            else
                perp_wall_dist = side_dist_y - delta_dist_y;

            if (perp_wall_dist > 0)  // Защита от отрицательных расстояний
            {
                int line_height = (int)(WIN_H / perp_wall_dist);
                int draw_start = -line_height / 2 + WIN_H / 2;
                if (draw_start < 0) draw_start = 0;
                int draw_end = line_height / 2 + WIN_H / 2;
                if (draw_end >= WIN_H) draw_end = WIN_H - 1;

                int color = 0xFF0000;
                if (side == 1)
                    color = color / 2;

                draw_vertical_line(data, x, draw_start, draw_end, color);
            }
        }
        x++;
    }
}*/
