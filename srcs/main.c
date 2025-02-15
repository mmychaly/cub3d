/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 03:12:00 by mmychaly          #+#    #+#             */
/*   Updated: 2025/02/14 22:45:56 by mmychaly         ###   ########.fr       */
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
	free_all(&data);
	return (0);
}
