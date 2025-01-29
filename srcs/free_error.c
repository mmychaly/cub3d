#include "../includes/cub.h"

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ; // Защита от NULL
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		strs[i] = NULL; // Обнуляем указатель после освобождения
		i++;
	}
	free(strs);
	strs = NULL; // Защита от dangling pointer
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
	if (data->map)
	{
		free_strs(data->map);
		data->map = NULL;
	}
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		data->mlx = NULL;
	}
}

int	error_wo_exit(t_data *data, char *message)
{
	if (message)
		printf("Error\n%s\n", message);
	free_all(data);
	return (-1);
}

void	error_with_exit(t_data *data, char *message, char *to_free, int *fd)
{
	if (message)
		printf("Error\n%s\n", message);
	if (to_free)
	{
		free(to_free);
		to_free = NULL;
	}
	if (fd && *fd >= 0) // Проверяем, открыт ли fd
	{
		close(*fd);
		*fd = -1;
	}
	free_all(data);
	exit(1);
}
