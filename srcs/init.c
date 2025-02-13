#include "../includes/cub.h"

void	init_data(t_data *data, char *map_path)
{
	ft_memset(data, 0, sizeof(t_data));
	data->f = -1;
	data->c = -1;
	parse_cub_file(data, map_path);
}
