/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 03:12:00 by mmychaly          #+#    #+#             */
/*   Updated: 2025/02/05 02:39:48 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
	{
		printf("Error\nUsage: %s <map.cub>\n", argv[0]);
		return (1);
	}
	ft_memset(&data, 0, sizeof(t_data));
	init_data(&data, argv[1]);
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
/*void extract_rgb(unsigned int color, unsigned char *r, unsigned char *g,
	unsigned char *b)
{
	*r = (color >> 16) & 0xff;
	*g = (color >> 8) & 0xff;
	*b = color & 0xff;
}*/