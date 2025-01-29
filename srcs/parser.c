#include "../includes/cub.h"

// Убирает пробелы только в конце строки (начало сохраняем!)
char *trim_end_spaces(char *str)
{
    int end = ft_strlen(str) - 1;

    while (end >= 0 && str[end] == ' ')
        end--;

    return ft_substr(str, 0, end + 1);
}

// Проверяет первую строку карты
int check_first_line(char *str)
{
    int i = 0;
    int counter_nb = 0;

    str = trim_end_spaces(str); // Убираем пробелы только в конце

    while (str[i] != '\n' && str[i] != '\0')
    {
        if (str[i] == '1')
            counter_nb++;
        else if (str[i] != ' ')
            return (0);
        i++;
    }

    return (counter_nb > 0); // Должен быть хотя бы один '1'
}

// Проверяет, является ли символ допустимым в карте
static int is_valid_char(char c)
{
    return (c == '1' || c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ');
}

// Проверяет, является ли позиция границей карты
static int is_border_position(int i, int j, t_data *data)
{
    return (i == 0 || i == data->map_height - 1 || j == 0 || j == data->map_width - 1);
}

// Проверяет, окружён ли пробел стенами
static int is_invalid_space(t_data *data, int i, int j)
{
    if (data->map[i][j] != ' ')
        return (0);
    
    if ((i > 0 && data->map[i - 1][j] != ' ' && data->map[i - 1][j] != '1') ||
        (i < data->map_height - 1 && data->map[i + 1][j] != ' ' && data->map[i + 1][j] != '1') ||
        (j > 0 && data->map[i][j - 1] != ' ' && data->map[i][j - 1] != '1') ||
        (j < data->map_width - 1 && data->map[i][j + 1] != ' ' && data->map[i][j + 1] != '1'))
        return (1);

    return (0);
}

// Проверяет карту на валидность
int get_map_dimensions(t_data *data, int i, int j, int *counter_pos)
{
    char c;

    c = data->map[i][j];

    if (!is_valid_char(c))
        return (error_wo_exit(data, "Invalid character in map"));

    if (is_border_position(i, j, data) && c != '1' && c != ' ')
        return (error_wo_exit(data, "Map is not surrounded by walls"));

    if (is_invalid_space(data, i, j))
        return (error_wo_exit(data, "Map contains open spaces"));

    if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
    {
        data->player.pos_x = (double)i + 0.5;
        data->player.pos_y = (double)j + 0.5;
        data->player.direction = c;
        (*counter_pos)++;
        if (*counter_pos > 1)
            return (error_wo_exit(data, "Multiple player start positions!"));
    }

    if (j + 1 > data->map_width)
        data->map_width = j + 1;

    return (0);
}

// Проверяет, что последний непустой символ в строке - '1'
int check_last_valid_char(char *line)
{
    int i;

    if (!line || *line == '\0' || *line == '\n')    // Если строка пустая, просто возвращаем 1 (так как это допустимо)

        return (1);

    i = ft_strlen(line) - 1;

    while (i >= 0 && (line[i] == ' ' || line[i] == '\n'))     // Пропускаем пробелы и символ новой строки в конце строки

        i--;

    if (i < 0)    // Если после удаления пробелов строка оказалась пустой — это нормально
        return (1);

    return (line[i] == '1');    // Последний значимый символ должен быть '1'

}


// Проверяет карту на валидность
int check_map_borders(t_data *data)
{
    int i = 0, j, counter_pos = 0;

    while (i < data->map_height)
    {
        if (!check_last_valid_char(data->map[i])) // Проверяем, что строка заканчивается на '1'
           { printf("%d\n", i);
			 return (error_wo_exit(data, "Map row does not end with wall!"));
		   }

        j = 0;
        while (data->map[i][j] != '\0' && data->map[i][j] != '\n')
        {
            if (get_map_dimensions(data, i, j, &counter_pos) == -1)
                return (-1);
            j++;
        }
        i++;
    }

    if (counter_pos == 0)
        return (error_wo_exit(data, "No player start position found!"));

    printf("Max width is %d\n", data->map_width);
    return (0);
}
