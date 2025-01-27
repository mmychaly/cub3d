#include "../includes/cub.h"

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
}

void	free_all(t_data *data)
{
	free(data->no);
	free(data->so);
	free(data->ea);
	free(data->we);
	free_strs(data->map);
}

void	window_error(t_data *data)
{
	perror("Error\n");
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	free_all(data);
	exit(EXIT_FAILURE);
}