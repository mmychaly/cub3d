#include "../includes/cub.h"

void	init_map(t_data *data,char *str,int fd,char **temp)//Сохраняем карту в масиве масивов
{
	int	i;
	
	i = 0;
	data->map_height += 1;
	temp = malloc(sizeof(char *) * (data->map_height + 1));
	if (temp == NULL)
		error_with_exit(data, "Memory allocation failed", str, &fd);
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
				error_with_exit(data, "malloc == NULL", str, &fd);
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
		error_with_exit(data, "Failed to open map file", NULL, NULL);
	str = get_next_line(fd);
	if (str == NULL)
		error_with_exit(data, "gnl == NULL", NULL, &fd);
	while (str && flag != 1)
	{
		temp = ft_split(str, ' ');
		if (temp == NULL)
			error_with_exit(data, "split == NULL", str, &fd);
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
			error_with_exit(data, "wrong map", str, &fd);
		if (data->f == -1 || data->c == -1)
			error_with_exit(data, "wrong colors", str, &fd);
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
		error_with_exit(data, "Wrong map after check_first_line", str, &fd);

	if (check_map_borders(data) == -1)//Небольшой чек карты и соханяем необходимые данные
		error_with_exit(data, "Wrong map", NULL, &fd);
	close(fd);
}
