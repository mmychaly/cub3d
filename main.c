/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 16:33:26 by mmychaly          #+#    #+#             */
/*   Updated: 2025/01/05 18:02:05 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub2d.h"

init_data(t_data *data, char **argv)
{
	int     fd;
	int     i;
	char    *str;
	char	**temp;
	
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		printf("Error\nmap doesn't open\n");
	str = get_next_line(fd);
	if (str == NULL)
		printf("Error\ngnl NULL\n");
	while (str)
	{
		if (ft_strncmp(str, "NO", 2))
		{
			temp = ft_split(str, ' ');
			data->no = ft_strdup(temp[1]);
		}
		else if (ft_strncmp(str, "SO", 2))
		{
			temp = ft_split(str, ' ');
			data->so = ft_strdup(temp[1]);
		}
		else if (ft_strncmp(str, "EA", 2))
		{
			temp = ft_split(str, ' ');
			data->ea = ft_strdup(temp[1]);
		}
		else if (ft_strncmp(str, "WE", 2))
		{
			temp = ft_split(str, ' ');
			data->we = ft_strdup(temp[1]);
		}
		else if (ft_strncmp(str, "F", 1))
		{
			temp = ft_split(str, ' ');
			data->f = define_colors(ft_strdup(temp[1]));
		}
		else if (ft_strncmp(str, "C", 1))
		{
			temp = ft_split(str, ' ');
			data->c = define_colors(ft_strdup(temp[1]));
		}
		else if (data->no && data->so && data->ea && data->we && data->f && data->c && str != '\n')
			init_map(str, data);
		
		free(str);
		if (temp != NULL)
		{
			free_all(temp);
			temp = NULL;
		}
		str = get_next_line(fd);
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
	init_data(&data, argv);
}
	