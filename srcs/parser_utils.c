/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 03:10:27 by mmychaly          #+#    #+#             */
/*   Updated: 2025/01/30 01:12:38 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

/*
remove_newline
убирает символ новой строки вконце строки
так как эта часть строки после сплита будет содержать в себе адрес файла.
*/
void remove_newline(char *str)
{
    int len;

	len = ft_strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

int	define_colors(char *str)//Трансформирует данные по цвету из строки в 24 битное значение сохраненное в переменной типа int 
{
    char **nb;
    int	r;
	int	g;
	int	b;
    int color;

    nb = ft_split(str, ',');
	if (nb == NULL)
	{
		printf("Error\nsplit in define_colors\n");
		return -1;
	}
    if (!nb || !nb[0] || !nb[1] || !nb[2] || nb[3])
	{
        free_strs(nb);
        return -1; 
    }
    r = atoi(nb[0]);//добавить проверку на int оверфлоу
    g = atoi(nb[1]);
    b = atoi(nb[2]);
	free_strs(nb);
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return -1;
    color = ((unsigned char)r << 16) | ((unsigned char)g << 8) | (unsigned char)b;
    return color;
}

void printf_map(t_data *data)//Просто вывод карты для визуализации парсера
{
	int i = 0;
	int i_2 = 0;
	printf("data.no %s\n", data->no);
	printf("data.so %s\n", data->so);
	printf("data.ea %s\n", data->ea);
	printf("data.we %s\n", data->we);
	printf("data.f %x\n", data->f);
	printf("data.c %x\n", data->c);
	while (i < data->map_height)
	{
		i_2 = 0;
		while (data->map[i][i_2] != '\n' && data->map[i][i_2] != '\0')
		{
			printf("%c",data->map[i][i_2]);
			i_2++;
		}
		printf("\n");
		i++;
	}
}