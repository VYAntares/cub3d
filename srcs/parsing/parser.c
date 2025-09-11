/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:28:38 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 18:57:12 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// static void	print_setup(t_setup *setup)
// {
// 	int	i;
// 	if (!setup)
// 	{
// 		printf("Setup is NULL\n");
// 		return ;
// 	}
// 	printf("=== SETUP CONFIGURATION ===\n\n");
// 	// Print textures
// 	printf("TEXTURES:\n");
// 	printf("  North: %s\n", setup->textures.north_texture ?
// 	 setup->textures.north_texture : "NULL");
// 	printf("  South: %s\n", setup->textures.south_texture ?
// 	 setup->textures.south_texture : "NULL");
// 	printf("  West:  %s\n", setup->textures.west_texture ?
// 	 setup->textures.west_texture : "NULL");
// 	printf("  East:  %s\n", setup->textures.east_texture ?
// 	 setup->textures.east_texture : "NULL");
// 	// Print colors
// 	printf("\nCOLORS:\n");
// 	printf("  Floor:   ");
// 	if (setup->floor.r == -1)
// 		printf("NOT SET\n");
// 	else
// 		printf("RGB(%d, %d, %d)\n", setup->floor.r,
// 	 setup->floor.g, setup->floor.b);
// 	printf("  Ceiling: ");
// 	if (setup->ceiling.r == -1)
// 		printf("NOT SET\n");
// 	else
// 		printf("RGB(%d, %d, %d)\n", setup->ceiling.r,
// 	 setup->ceiling.g, setup->ceiling.b);
// 	// Print map info
// 	printf("\nMAP INFO:\n");
// 	printf("  Width:  %d\n", setup->map_width);
// 	printf("  Height: %d\n", setup->map_height);
// 	printf("  Map:    %s\n", setup->map ? "LOADED" : "NULL");
// 	// Print actual map if it exists
// 	if (setup->map)
// 	{
// 		printf("\nMAP CONTENT:\n");
// 		i = 0;
// 		while (i < setup->map_height && setup->map[i])
// 		{
// 			printf("  [%2d] [strlen : %2zu] :%s\n", i,
// 	 ft_strlen(setup->map[i]), setup->map[i]);
// 			i++;
// 		}
// 	}
// 	printf("\n=== END SETUP ===\n");
// }

int	parse_textures(char *trimmed, char **texture_path)
{
	*texture_path = ft_substr(trimmed, 2, (ft_strlen(trimmed) - 2));
	if (!*texture_path || ft_strlen(*texture_path) == 0)
	{
		free(*texture_path);
		*texture_path = NULL;
		return (1);
	}
	return (0);
}

int	is_all_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	free_rgb(char **rgb)
{
	int	i;

	i = 0;
	while (rgb[i])
		free(rgb[i++]);
	free(rgb);
}

int	parse_color(char *trimmed, t_color *color)
{
	char	**rgb;
	char	*color_str;

	color_str = ft_substr(trimmed, 1, (ft_strlen(trimmed) - 1));
	if (!color_str)
		return (1);
	rgb = ft_split(color_str, ',');
	free(color_str);
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
	{
		if (rgb)
			free_rgb(rgb);
		return (1);
	}
	if (!is_all_digits(rgb[0]) || !is_all_digits(rgb[1])
		|| !is_all_digits(rgb[2]))
		return (free_rgb(rgb), 1);
	color->r = ft_atoi(rgb[0]);
	color->g = ft_atoi(rgb[1]);
	color->b = ft_atoi(rgb[2]);
	free_rgb(rgb);
	if (color->r < 0 || color->r > 255 || color->g < 0
		|| color->g > 255 || color->b < 0 || color->b > 255)
		return (1);
	return (0);
}

int	parse_config(char *line, t_setup *setup)
{
	char	*trimmed;
	int		result;

	trimmed = trim_line(line);
	if (ft_strlen(trimmed) == 0)
		return (free(trimmed), 0);
	if (ft_strncmp(trimmed, "NO", 2) == 0)
		result = parse_textures(trimmed, &setup->textures.north_texture);
	else if (ft_strncmp(trimmed, "WE", 2) == 0)
		result = parse_textures(trimmed, &setup->textures.west_texture);
	else if (ft_strncmp(trimmed, "SO", 2) == 0)
		result = parse_textures(trimmed, &setup->textures.south_texture);
	else if (ft_strncmp(trimmed, "EA", 2) == 0)
		result = parse_textures(trimmed, &setup->textures.east_texture);
	else if (ft_strncmp(trimmed, "F", 1) == 0)
		result = parse_color(trimmed, &setup->floor);
	else if (ft_strncmp(trimmed, "C", 1) == 0)
		result = parse_color(trimmed, &setup->ceiling);
	else if (trimmed[0] == '1' || trimmed[0] == '0')
		result = MAP_FOUND;
	else
		result = 1;
	free(trimmed);
	return (result);
}

int	fill_up_void_space(t_setup *setup, int i)
{
	int		j;
	int		len;
	char	*new_line;

	len = ft_strlen(setup->map[i]);
	if (len < setup->map_width)
	{
		new_line = malloc(setup->map_width + 1);
		if (!new_line)
			return (1);
		j = 0;
		while (j < setup->map_width)
		{
			if (j < len)
				new_line[j] = setup->map[i][j];
			else
				new_line[j] = ' ';
			j++;
		}
		new_line[j] = '\0';
		free(setup->map[i]);
		setup->map[i] = new_line;
	}
	return (0);
}

int	normalize_map(t_setup *setup)
{
	int		i;

	if (validate_config_complete(setup) == 1)
		return (1);
	i = 0;
	find_max_width(setup);
	while (i < setup->map_height)
	{
		if (fill_up_void_space(setup, i) == 1)
			return (1);
		i++;
	}
	return (0);
}

int	realloc_shit(t_setup *setup, int *map_count, int *map_capacity, char *line)
{
	if (*map_count >= *map_capacity)
	{
		*map_capacity *= 2;
		setup->map = ft_realloc_array(setup->map, *map_count,
				sizeof(char *) * (*map_capacity));
		if (!setup->map)
			return (1);
	}
	if (substr_map_line_wo_bn(setup, line, *map_count) == 1)
		return (1);
	(*map_count)++;
	return (0);
}

int	write_map(t_setup *setup, char *line, int fd)
{
	int	map_count;
	int	map_capacity;

	map_capacity = 100;
	map_count = 0;
	setup->map = malloc(sizeof(char *) * map_capacity);
	if (!setup->map)
		return (1);
	if (substr_map_line_wo_bn(setup, line, map_count) == 1)
		return (1);
	map_count++;
	free(line);
	line = get_next_line(fd);
	while (line)
	{
		if (realloc_shit(setup, &map_count, &map_capacity, line) == 1)
			return (1);
		free(line);
		line = get_next_line(fd);
	}
	setup->map[map_count] = NULL;
	setup->map_height = map_count;
	return (normalize_map(setup));
}

int	parser(t_setup *setup, char *filename)
{
	int		fd;
	char	*line;
	int		result;

	init_setup(setup);
	fd = validate_file(filename);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		result = parse_config(line, setup);
		if (result == 1)
			return (free(line), close(fd), clean_up_gnl(fd), 1);
		if (result == MAP_FOUND)
		{
			if (write_map(setup, line, fd) == 1)
				return (close(fd), clean_up_gnl(fd), 1);
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	print_setup(setup);
	return (close(fd), clean_up_gnl(fd), validate_map(setup));
}
