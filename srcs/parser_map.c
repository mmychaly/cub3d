#include "../includes/cub.h"

static char	**append_line_to_map(t_data *data, char **old_map, char *line)
{
	char	**new_map;
	int		i;

	data->map_height++;
	new_map = (char **)malloc(sizeof(char *) * (data->map_height + 1));
	if (!new_map)
		error_with_exit(data, "malloc failed in append_line_to_map", line, NULL);
	new_map[data->map_height] = NULL;
	i = 0;
	while (old_map && old_map[i])
	{
		new_map[i] = old_map[i];
		i++;
	}
	new_map[i] = line;
	free(old_map);
	return (new_map);
}

void parse_map_lines(t_data *data, int fd)
{
    char *trimmed;
    int map_started = 0;
    int map_ended = 0;

    data->map = NULL;
    data->map_height = 0;

    while ((data->line = get_next_line(fd)))
    {
        remove_newline(data->line );  // Убираем завершающий символ '\n'
        trimmed = ft_strtrim(data->line , " \t\r\n");  // удаляем все пробельные символы с начала и конца
        if (!map_started)
        {
            if (trimmed[0] == '\0')  // строка пуста
            {
                free(trimmed);
                free(data->line);
                data->line = NULL;
                continue;
            }
            else
            {
                map_started = 1;
                data->map = append_line_to_map(data, data->map, data->line );
                free(trimmed);
                continue;
            }
        }
        else
        {
            if (trimmed[0] == '\0')
            {
                map_ended = 1;
                free(trimmed);
                free(data->line);
                data->line = NULL;
                continue;
            }
            else
            {
                if (map_ended)
                    error_with_exit(data, "Invalid map: non-empty line after empty line", trimmed, NULL);
                data->map = append_line_to_map(data, data->map, data->line );

            }
        }
        free(trimmed);
    }
    /* Определяем максимальную длину строки (map_width) */
    {
        int i, len;
        data->map_width = 0;
        for (i = 0; i < data->map_height; i++) {
            len = ft_strlen(data->map[i]);
            if (len > data->map_width)
                data->map_width = len;
        }
    }
    if (data->map_height == 0)
        error_with_exit(data, "Map is empty", NULL, NULL);
}


void pad_map(t_data *data)
{
    int i, j;
    char *new_line;
    int len;

    for (i = 0; i < data->map_height; i++) {
         len = ft_strlen(data->map[i]);
         if (len < data->map_width) {
             new_line = malloc(data->map_width + 1);
             if (!new_line)
                 error_with_exit(data, "malloc failed in pad_map", NULL, NULL);
             j = 0;
             while (j < len) {
                 new_line[j] = data->map[i][j];
                 j++;
             }
             while (j < data->map_width)
                 new_line[j++] = ' ';
             new_line[j] = '\0';
             free(data->map[i]);
             data->map[i] = new_line;
         }
    }
}

int check_map_closure(t_data *data) // Тут надо обсудить, если есть незакрытые стены, но игрок до туда не дойдет - блокируем
{
    int i, j, di, dj;

    for (i = 0; i < data->map_height; i++) {
        for (j = 0; j < data->map_width; j++) {
            if (data->map[i][j] == '0' || data->map[i][j] == 'N' ||
                data->map[i][j] == 'S' || data->map[i][j] == 'E' ||
                data->map[i][j] == 'W')
            {
                for (di = -1; di <= 1; di++) {
                    for (dj = -1; dj <= 1; dj++) {
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni < 0 || nj < 0 || ni >= data->map_height || nj >= data->map_width)
                            return error_wo_exit("Map is not closed (border reached)");
                        if (data->map[ni][nj] == ' ')
                            return error_wo_exit("Map is not closed (empty space adjacent)");
                    }
                }
            }
        }
    }
    return 0;
}



/* -------------------------------------------------------------------------
 * check_map_validity:
 * Здесь вызываем все проверки:
 * 1) Наличие игрока, и что он один.
 * 2) Корректность символов.
 * 3) Закрытость карты (нет "дыр" по краям и внутри).
 * и т.д.
 * ------------------------------------------------------------------------ */
int	check_map_validity(t_data *data)
{
	int i;
	int j;
	int player_count;

	player_count = 0;
	i = -1;
	while (++i < data->map_height)
	{
		j = 0;
		while (data->map[i][j])
		{
			if (!is_valid_map_char(data->map[i][j]))
			{
				printf("Char is %c\n", data->map[i][j]);
				return (error_wo_exit("Invalid character in map"));
			}
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S'
				|| data->map[i][j] == 'E' || data->map[i][j] == 'W')
			{
				data->player.pos_x = i + 0.5;
				data->player.pos_y = j + 0.5;
				data->player.direction = data->map[i][j];
				player_count++;
			}
			j++;
		}
	}
	if (player_count == 0)
		return (error_wo_exit("No player start position found"));
	if (player_count > 1)
		return (error_wo_exit("Multiple player start positions"));
	/* Дополнительно проверяем “замкнутость” карты */
	/* Можно либо строгие проверки крайних строк, либо flood fill, etc. */
	/* Здесь для примера вынесем отдельную функцию: */
	return (0);
}