#include "../includes/cub.h"

void init_player_direction(t_data *data)
{
    // Инициализация начального направления и плоскости камеры
    if (data->player.direction == 'N')
    {
        data->player.dir_x = 0.0;
        data->player.dir_y = -1.0;
        data->player.plane_x = 0.66;
        data->player.plane_y = 0.0;
    }
    else if (data->player.direction == 'S')
    {
        data->player.dir_x = 0.0;
        data->player.dir_y = 1.0;
        data->player.plane_x = -0.66;
        data->player.plane_y = 0.0;
    }
    else if (data->player.direction == 'E')
    {
        data->player.dir_x = 1.0;
        data->player.dir_y = 0.0;
        data->player.plane_x = 0.0;
        data->player.plane_y = 0.66;
    }
    else if (data->player.direction == 'W')
    {
        data->player.dir_x = -1.0;
        data->player.dir_y = 0.0;
        data->player.plane_x = 0.0;
        data->player.plane_y = -0.66;
    }
}

void draw_textured_line(t_data *data, int x, int draw_start, int draw_end, t_img_data *texture, double wall_x, int line_height)
{
    int tex_size = TEX_SIZE;// Используем для размера текстуры
    int tex_x = (int)(wall_x * tex_size);// Вычисляем x координату на текстуре с более точным приведением типов
    double step = (double)tex_size / line_height;// Вычисляем шаг текстуры с большей точностью
    double tex_pos = (draw_start - ((WIN_H - line_height) / 2.0)) * step;// Вычисляем начальную позицию текстуры с учетом смещения
    
    int y = draw_start;
    while (y <= draw_end)
    {
        int tex_y = (int)tex_pos & (tex_size - 1);// Обеспечиваем корректную y-координату текстуры
        tex_pos += step;
        unsigned int color = get_texture_color(texture, tex_x, tex_y);// Получаем цвет из текстуры
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
        if (ray_dir_x < 0) //луч движется влево
        {
            step_x = -1;
            side_dist_x = (data->player.pos_x - map_x) * delta_dist_x;
        }
        else //луч движется вправо
        {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - data->player.pos_x) * delta_dist_x;
        }
        if (ray_dir_y < 0)//луч движется вверх
        {
            step_y = -1;
            side_dist_y = (data->player.pos_y - map_y) * delta_dist_y;
        }
        else//луч движется вниз
        {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - data->player.pos_y) * delta_dist_y;
        }

        // 6. DDA алгоритм

        int side;//Флаг для стены горизонатльная/вертикальная 
        while (data->map[map_y][map_x] != '1')//пока стена не найдена 
        {
            if (side_dist_x < side_dist_y)//Если вертикальная грань ближе 
            {
                // Перемещаемся по горизонтали
                side_dist_x += delta_dist_x;//Увеличиваем дистанцию до следующей горизонатльной грани 
                map_x += step_x;// Перемещаемся на следующую сетку по горизонтали 
                side = 0;// Отмечаем что пересекли вертикальную грань
            }
            else// Если ближайшая горизонтальная грань
            {
                // Перемещаемся по вертикали
                side_dist_y += delta_dist_y;// Увеличиваем расстояние до следующей вертикальной грани
                map_y += step_y; // Перемещаемся на следующую сетку по вертикали
                side = 1;// Отмечаем что пересекли горизонтальную грань
            }
        }

        // 7. Вычисление расстояния до стены
        double wall_dist;
        if (side == 0)
            wall_dist = (side_dist_x - delta_dist_x);
        else
            wall_dist = (side_dist_y - delta_dist_y);

        // 8. Вычисление высоты линии для отрисовки
        int line_height = (int)(WIN_H / wall_dist);
        int draw_start = WIN_H / 2 - line_height / 2;
        if (draw_start < 0)
            draw_start = 0;
        int draw_end = WIN_H / 2 + line_height / 2;
        if (draw_end >= WIN_H)
            draw_end = WIN_H - 1;//Исли убрать -1 то пропадает проблема с полосой в пиксель 
        double wall_x;
        if (side == 0)
//		    wall_x = data->player.pos_y + wall_dist * data->player.dir_y;//Пропадают тестуры спереди
            wall_x = data->player.pos_y + wall_dist * ray_dir_y;//Хорошие текстуры
	    else
           wall_x = data->player.pos_x + wall_dist * ray_dir_x;
//		    wall_x = data->player.pos_x + wall_dist * data->player.dir_x;
	    wall_x -= floor(wall_x);
        t_img_data *current_texture;
        if (side == 0 && ray_dir_x < 0)//Западная стена 
            current_texture = &data->west;
        else if (side == 0 && ray_dir_x > 0)//Восточная стена
            current_texture = &data->east;
        else if (side == 1 && ray_dir_y < 0)//Северная
            current_texture = &data->north;
        else if (side == 1 && ray_dir_y > 0)//Южная стена
            current_texture = &data->south;

        // 10. Отрисовка вертикальной линии
        draw_textured_line(data, x, draw_start, draw_end, current_texture, wall_x, line_height);
        x++;
    }
}


        // Применяем затемнение с плавным переходом
/*       if (wall_dist > 1.0)
        {
            double darkness = 1.0 / wall_dist;
            if (darkness > 1.0) 
                darkness = 1.0;
            unsigned char r = (unsigned char)(((color >> 16) & 0xFF) * darkness);
            unsigned char g = (unsigned char)(((color >> 8) & 0xFF) * darkness);
            unsigned char b = (unsigned char)((color & 0xFF) * darkness);
            
            color = (r << 16) | (g << 8) | b;
        }*/