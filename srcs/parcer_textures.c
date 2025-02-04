#include "../includes/cub.h"

/*
** Проверяем, что заданы все 6 обязательных полей: 4 текстуры и 2 цвета.
*/
static int	is_all_textures_and_colors_set(t_data *data)
{
	if (!data->no || !data->so || !data->we || !data->ea)
		return (0);
	if (data->f < 0 || data->c < 0)
		return (0);
	return (1);
}

/*
** Проверка файла текстуры:
** - Путь не NULL
** - Расширение должно быть ".xpm"
** - Файл должен существовать и быть доступен для чтения (R_OK)
*/
void	check_texture_file(char *path, t_data *data, char **split)
{
	int	len;

	if (!path)
		error_with_exit(data, "Texture path is NULL", NULL, NULL);
	len = ft_strlen(path);
	if (len < 4 || ft_strncmp(path + len - 4, ".xpm", 4) != 0)
		error_with_exit(data, "Invalid texture file extension (should be .xpm)", NULL, split);
	if (access(path, R_OK) != 0)
		error_with_exit(data, "Texture not exist/don't have permiss", NULL, split);
}

/*
** Удаляет все пробелы из строки s.
*/
char	*remove_all_spaces(const char *s)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = malloc(ft_strlen(s) + 1);
	if (!res)
		return (NULL);
	while (s[i])
	{
		if (s[i] != ' ')
			res[j++] = s[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

/*
** Объединяет токены из массива, начиная с индекса start_index.
*/
char	*join_tokens(char **tokens, int start_index)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = start_index;
	while (tokens[i])
	{
		tmp = ft_strjoin(result, tokens[i]);
		free(result);
		result = tmp;
		i++;
	}
	return (result);
}

/*
** Вспомогательная функция для парсинга текстур (NO, SO, WE, EA).
*/
static void	parse_texture_line(t_data *data, char **split)
{
	if (ft_strncmp(split[0], "NO", 3) == 0)
	{
		if (data->no != NULL)
			error_with_exit(data, "Duplicate NO texture", NULL, split);
		data->no = ft_strdup(split[1]);
		check_texture_file(data->no, data, split);
	}
	else if (ft_strncmp(split[0], "SO", 3) == 0)
	{
		if (data->so != NULL)
			error_with_exit(data, "Duplicate SO texture", NULL, split);
		data->so = ft_strdup(split[1]);
		check_texture_file(data->so, data, split);
	}
	else if (ft_strncmp(split[0], "WE", 3) == 0)
	{
		if (data->we != NULL)
			error_with_exit(data, "Duplicate WE texture", NULL, split);
		data->we = ft_strdup(split[1]);
		check_texture_file(data->we, data, split);
	}
	else if (ft_strncmp(split[0], "EA", 3) == 0)
	{
		if (data->ea != NULL)
			error_with_exit(data, "Duplicate EA texture", NULL, split);
		data->ea = ft_strdup(split[1]);
		check_texture_file(data->ea, data, split);
	}
	else
		error_with_exit(data, "Invalid texture identifier", NULL, split);
}

/*
** Вспомогательная функция для парсинга цветов (F для пола, C для потолка).
** Объединяет токены, удаляет пробелы и вызывает define_colors.
*/
static void	parse_color_line(t_data *data, char **split, char id)
{
	char	*joined;
	char	*color_str;

	joined = join_tokens(split, 1);
	color_str = remove_all_spaces(joined);
	free(joined);
	if (id == 'F')
	{
		if (data->f != -1)
			error_with_exit(data, "Duplicate floor color", color_str, split);
		data->f = define_colors(color_str);
		if (data->f == -1)
			error_with_exit(data, "Invalid floor color", color_str, split);
	}
	else if (id == 'C')
	{
		if (data->c != -1)
			error_with_exit(data, "Duplicate ceiling color", color_str, split);
		data->c = define_colors(color_str);
		if (data->c == -1)
			error_with_exit(data, "Invalid ceiling color", color_str, split);
	}
	else
		error_with_exit(data, "Invalid color identifier", color_str, split);
	free(color_str);
}

/*
** Разбирает одну строку заголовка.
*/
void	parse_single_line(t_data *data, char *line)
{
	char	**split;

	if (!line || !(*line))
		return ;
	split = ft_split(line, ' ');
	if (!split || !split[0])
	{
		free_strs(split);
		return ;
	}
	if (ft_strncmp(split[0], "NO", 3) == 0 || ft_strncmp(split[0], "SO", 3) == 0 ||
		ft_strncmp(split[0], "WE", 3) == 0 || ft_strncmp(split[0], "EA", 3) == 0)
		parse_texture_line(data, split);
	else if (ft_strncmp(split[0], "F", 2) == 0)
		parse_color_line(data, split, 'F');
	else if (ft_strncmp(split[0], "C", 2) == 0)
		parse_color_line(data, split, 'C');
	else
		error_with_exit(data, "Invalid identifier in header", NULL, split);
	free_strs(split);
}

/*
** Считываем строки заголовка (текстуры и цвета) до момента,
** пока не будут заданы все необходимые поля.
*/
void	parse_textures_and_colors(t_data *data, int fd)
{
	char	*line;
	int		end_of_header;

	end_of_header = 0;
	while (!end_of_header)
	{
		line = get_next_line(fd);
		data->line = line;
		if (!line)
			error_with_exit(data, "Empty .cub file", NULL, NULL);
		remove_newline(line); /* Убираем символ '\n' */
		parse_single_line(data, line);
		free(line);
		end_of_header = is_all_textures_and_colors_set(data);
		if (end_of_header)
			break ;
	}
}