#include "../includes/cub.h"

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ; // Защита от NULL
	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		strs[i] = NULL; // Обнуляем указатель после освобождения
		i++;
	}
	free(strs);
	strs = NULL; // Защита
}

void	free_all(t_data *data)
{
	if (!data)
		return ; // Защита от NULL
	if (data->no)
	{
		free(data->no);
		data->no = NULL;
	}
	if (data->so)
	{
		free(data->so);
		data->so = NULL;
	}
	if (data->ea)
	{
		free(data->ea);
		data->ea = NULL;
	}
	if (data->we)
	{
		free(data->we);
		data->we = NULL;
	}
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	if (data->map)
	{
		free_strs(data->map);
		data->map = NULL;
	}
}

void	free_gnl(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		line = get_next_line(fd);
	}
}

int	error_wo_exit(char *message)
{
	if (message)
		printf("Error\n%s\n", message);
	return (-1);
}

void	error_with_exit(t_data *data, char *msg, char *to_free, char **split) //Аргументы для очистки актуальны для парсера
{
	if (msg)
		printf("Error\n%s\n", msg);
	if(split)
		free_strs(split);
	if (to_free)
		free(to_free);
	if (data->fd > 0)
	{
		free_gnl(data->fd);
		close(data->fd);
	}
	free_all(data);
	exit(1);
}

void	window_error(t_data *data)
{
	perror("Error\n");
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	free_all(data);
	exit(EXIT_FAILURE);
}