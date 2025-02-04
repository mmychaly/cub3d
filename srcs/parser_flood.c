#include "../includes/cub.h"

int	**allocate_visited(int height, int width)
{
	int	**visited;
	int	i;
			int j;

	visited = malloc(sizeof(int *) * height);
	if (!visited)
		return (NULL);
	for (i = 0; i < height; i++)
	{
		visited[i] = malloc(sizeof(int) * width);
		if (!visited[i])
		{
			for (j = 0; j < i; j++)
				free(visited[j]);
			free(visited);
			return (NULL);
		}
		memset(visited[i], 0, sizeof(int) * width);
	}
	return (visited);
}

void	free_visited(int **visited, int height)
{
	int	i;

	for (i = 0; i < height; i++)
		free(visited[i]);
	free(visited);
}

int	flood_fill(t_data *data, int i, int j, int **visited)
{
	/* Если вышли за пределы карты – ошибка */
	if (i < 0 || j < 0 || i >= data->map_height || j >= data->map_width)
		return (1);
	/* Если ячейка – пробел, значит заливка попала в «неопределённую» область */
	if (data->map[i][j] == ' ')
		return (1);
	/* Если стена – останавливаем заливку */
	if (data->map[i][j] == '1')
		return (0);
	if (visited[i][j])
		return (0);
	visited[i][j] = 1;
	if (flood_fill(data, i - 1, j, visited))
		return (1);
	if (flood_fill(data, i + 1, j, visited))
		return (1);
	if (flood_fill(data, i, j - 1, visited))
		return (1);
	if (flood_fill(data, i, j + 1, visited))
		return (1);
	return (0);
}
int	check_map_closure_flood(t_data *data)
{
	int	**visited;
	int	start_i;
	int	start_j;

	start_i = (int)data->player.pos_x;
	start_j = (int)data->player.pos_y;
	visited = allocate_visited(data->map_height, data->map_width);
	if (!visited)
		error_with_exit(data, "Memory allocation failed in flood fill", NULL,
			NULL);
	if (flood_fill(data, start_i, start_j, visited))
	{
		free_visited(visited, data->map_height);
		return (error_wo_exit("Invalid map closure: flood fill reached border or space"));
	}
	free_visited(visited, data->map_height);
	return (0);
}
