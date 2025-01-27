/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 03:12:00 by mmychaly          #+#    #+#             */
/*   Updated: 2025/01/27 20:43:06 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	check_first_line(char *str)//Функция проверяет является переданная строка первой строкой карты
{
	int	i;
	int	counter_space;
	int counter_nb;

	i = 0;
	counter_space = 0;
	counter_nb = 0;
	while(str[i] != '\n' && str[i] != '\0')
	{
		if (str[i] == ' ')
			counter_space += 1;
		else if (str[i] == '1')
			counter_nb += 1;
		else
			return (0);
		i++;
	}
	if (counter_nb > 0 && counter_space >= 0)//Нужен хотя бы один символ 1 если нет то это не валидная строка и карта
		return (1);
	return (0);
}
/*
get_map_dimensions
-проверяет карту на посторонние символы
-сохраняет размер самой широкой строки 
-сохраняет точку старта игрока а так же его вектор
-проверяет что бы точек старта не было больше 1 или меньше 1
*/
int get_map_dimensions(t_data *data)
{
    int i;
    int i_2;
    int len;
    int counter_pos;

    i = 0;
    i_2 = 0;
	len = 0;
    data->map_width = 0;
	counter_pos = 0;
    while (i < data->map_height)
    {
		i_2 = 0;
        while (data->map[i][i_2] != '\n' && data->map[i][i_2] != '\0')//Проверяем карту на символы а так же позиционируем игрока
        {
            if (data->map[i][i_2] != '0' && data->map[i][i_2] != '1' && data->map[i][i_2] != ' ' &&
                data->map[i][i_2] != 'N' && data->map[i][i_2] != 'S' && data->map[i][i_2] != 'E' && data->map[i][i_2] != 'W')
				return (-1);//Есть лишние символы
            if (data->map[i][i_2] == 'N' || data->map[i][i_2] == 'S' || data->map[i][i_2] == 'E' || data->map[i][i_2] == 'W')
            {
                data->player.pos_x = (double)i + 0.5;
				data->player.pos_y = (double)i_2 + 0.5;
				data->player.direction = data->map[i][i_2];
				counter_pos++;
				if (counter_pos > 1)
					return (-1);//Слишком много точек старта
            }
            i_2++;
        }
        len = ft_strlen(data->map[i]);//Вычесляем самую широкую строку
        if (len > data->map_width)
            data->map_width = len;
        i++;
    }
	if (counter_pos == 0)
		return (-1);//нет точки старта
	return (0);
}

void	init_map(t_data *data,char *str,int fd,char **temp)//Сохраняем карту в масиве масивов
{
	int	i;
	
	i = 0;
	data->map_height += 1;
	temp = malloc(sizeof(char *) * (data->map_height + 1));
	if (temp == NULL)
	{
		printf("Error\nmalloc == NULL\n");
		close(fd);
		free(str);
		exit(1);
	}
	temp[data->map_height] = NULL;
	temp[i] = str;
	while (str != NULL)
	{
		data->map = temp;
		str = get_next_line(fd);
		if (str != NULL)
		{
			i = 0;
			data->map_height += 1;
			temp = malloc(sizeof(char *) * (data->map_height + 1));
			if (temp == NULL)
			{
				printf("Error\nmalloc == NULL\n");
				close(fd);
				free(str);
				if (data->map != NULL)
					free_strs(data->map);
				exit(1);
			}
			temp[data->map_height] = NULL;
			while (i < (data->map_height - 1))
			{
				temp[i] = ft_strdup(data->map[i]);
				i++;
			}
			temp[i] = str;
			free_strs(data->map);
		}
	}
	data->map = temp;
}

void	init_data(t_data *data, char **argv)
{
	int     fd;
	char    *str;
	char	**temp;
	int		flag;//Поможет нам отделить дополнительные данные от карты 

	flag = 0;	
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		printf("Error\nmap doesn't open\n");
		exit(1);
	}
	str = get_next_line(fd);
	if (str == NULL)
	{
		printf("Error\ngnl == NULL\n");
		close(fd);
		exit(1);
	}
	while (str && flag != 1)
	{
		temp = ft_split(str, ' ');
		if (temp == NULL)
		{
			printf("Error\nsplit == NULL\n");
			close(fd);
			free(str);
			exit(1);
		}
		if (temp[0][0] != '\n')
			remove_newline(temp[1]);
		if (!ft_strncmp(temp[0], "NO", 3))
			data->no = ft_strdup(temp[1]);
		else if (!ft_strncmp(temp[0], "SO", 3))
			data->so = ft_strdup(temp[1]);
		else if (!ft_strncmp(temp[0], "EA", 3))
			data->ea = ft_strdup(temp[1]);
		else if (!ft_strncmp(temp[0], "WE", 3))
			data->we = ft_strdup(temp[1]);
		else if (!ft_strncmp(temp[0], "F", 2))
			data->f = define_colors(temp[1]);
		else if (!ft_strncmp(temp[0], "C", 2))
			data->c = define_colors(temp[1]);
		else if (!ft_strncmp(temp[0], "\n", 1))
			flag = 0;
		else
		{
			printf("Error\nwrong map\n");
			close(fd);
			free(str);
			free_strs(temp);
			exit(1);
		}
		if (data->f == -1 || data->c == -1)
		{
			printf("Error\nwrong colors\n");
			close(fd);
			free(str);
			free_strs(temp);
			exit(1);
		}
		if (data->no && data->so && data->ea && data->we && data->f && data->c)
			flag = 1;
		free(str);
		free_strs(temp);
		str = get_next_line(fd);
	}
	while(str[0] == '\n')//Пропускаем пустые строки 
	{
		free(str);
		str = get_next_line(fd);
	}
	if (check_first_line(str))//Проверка строки что бы убедиться что началась карта
		init_map(data,str,fd,temp);//Создаем карту
	else
	{
		printf("Error\nwrong map\n");
		close(fd);
		free(str);
		exit(1);
	}
	if (get_map_dimensions(data) == -1)//Небольшой чек карты и соханяем необходимые данные
	{
		printf("Error\nwrong map\n");
		close(fd);
		free_strs(data->map);
		exit(1);
	}
	close(fd);
}

int main(int argc, char **argv)
{
	t_data data;
	if (argc != 2)
	{
		printf("Error\nwrong numbers of arguments\n");
		return (1);
	}
	ft_memset(&data, 0, sizeof(t_data));
	init_data(&data, argv);
	printf_map(&data);
	creat_game(&data);

	mlx_destroy_image(data.mlx, data.img);
	mlx_destroy_window(data.mlx, data.win);
	mlx_destroy_display(data.mlx);
	free(data.mlx);
	free_all(&data);
	return (0);
}


// Извлечение компонентов из числа
/*void extract_rgb(unsigned int color, unsigned char *r, unsigned char *g, unsigned char *b)
{
    *r = (color >> 16) & 0xff;
    *g = (color >> 8) & 0xff;
    *b = color & 0xff;
}*/