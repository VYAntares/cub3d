/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:28:38 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/07 22:46:01 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	parse_textures(char *trimmed, char **texture_path)
{
	*texture_path = ft_substr(trimmed, 2, (ft_strlen(trimmed) - 2));
	if (!*texture_path || ft_strlen(*texture_path) == 0)
	{
		free(*texture_path);
		*texture_path = NULL;
		return (ERROR_TEXTURE);
	}
	return (0);
}

int	parse_color(char *trimmed, t_color *color)
{
	char 	**rgb;
	char 	*color_str;
	int		i;
	
	color_str = ft_substr(trimmed, 1, (ft_strlen(trimmed) - 1));
	if (!color_str)
		return (ERROR_COLOR);
	rgb = ft_split(color_str, ',');
	free(color_str);
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
	{
		if (rgb)
		{
			i = 0;
			while (rgb[i])
				free(rgb[i++]);
			free(rgb);
		}
		return (ERROR_COLOR);
	}
	color->r = ft_atoi(rgb[0]);
	color->g = ft_atoi(rgb[1]);
	color->b = ft_atoi(rgb[2]);
	i = 0;
	while (rgb[i])
		free(rgb[i++]);
	free(rgb);
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		return (ERROR_COLOR);
	return (0);
}

int	parse_config(char *line, t_setup *setup)
{
	char	*trimmed;

	trimmed = trim_line(line);
	if (ft_strlen(trimmed) == 0)
		return (0);
	if (ft_strncmp(trimmed, "NO", 2) == 0)
		return (parse_textures(trimmed, &setup->textures.north_texture));
	else if (ft_strncmp(trimmed, "WE", 2) == 0)
		return (parse_textures(trimmed, &setup->textures.west_texture));
	else if (ft_strncmp(trimmed, "SO", 2) == 0)
		return (parse_textures(trimmed, &setup->textures.south_texture));
	else if (ft_strncmp(trimmed, "EA", 2) == 0)
		return (parse_textures(trimmed, &setup->textures.east_texture));
	else if (ft_strncmp(trimmed, "F", 1) == 0)
		return (parse_color(trimmed, &setup->floor));
	else if (ft_strncmp(trimmed, "C", 1) == 0)
		return (parse_color(trimmed, &setup->ceiling));
	else if (trimmed[0] == '1' || trimmed[0] == '0')
		return (MAP_FOUND);
	return (0);
}

static void	print_setup(t_setup *setup)
{
	int	i;

	if (!setup)
	{
		printf("Setup is NULL\n");
		return ;
	}

	printf("=== SETUP CONFIGURATION ===\n\n");

	// Print textures
	printf("TEXTURES:\n");
	printf("  North: %s\n", setup->textures.north_texture ? setup->textures.north_texture : "NULL");
	printf("  South: %s\n", setup->textures.south_texture ? setup->textures.south_texture : "NULL");
	printf("  West:  %s\n", setup->textures.west_texture ? setup->textures.west_texture : "NULL");
	printf("  East:  %s\n", setup->textures.east_texture ? setup->textures.east_texture : "NULL");

	// Print colors
	printf("\nCOLORS:\n");
	printf("  Floor:   ");
	if (setup->floor.r == -1)
		printf("NOT SET\n");
	else
		printf("RGB(%d, %d, %d)\n", setup->floor.r, setup->floor.g, setup->floor.b);
	
	printf("  Ceiling: ");
	if (setup->ceiling.r == -1)
		printf("NOT SET\n");
	else
		printf("RGB(%d, %d, %d)\n", setup->ceiling.r, setup->ceiling.g, setup->ceiling.b);

	// Print map info
	printf("\nMAP INFO:\n");
	printf("  Width:  %d\n", setup->map_width);
	printf("  Height: %d\n", setup->map_height);
	printf("  Map:    %s\n", setup->map ? "LOADED" : "NULL");

	// Print actual map if it exists
	if (setup->map)
	{
		printf("\nMAP CONTENT:\n");
		i = 0;
		while (i < setup->map_height && setup->map[i])
		{
			printf("  [%2d] [strlen : %2zu] %s\n", i, ft_strlen(setup->map[i]), setup->map[i]);
			i++;
		}
	}

	printf("\n=== END SETUP ===\n");
}

void	*ft_realloc_array(char **old_array, int old_count, size_t new_size)
{
	char	**new_array;
	int		i;

	new_array = malloc(new_size);
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < old_count)
	{
		new_array[i] = old_array[i];
		i++;
	}
	free(old_array);
	return (new_array);
}

void	find_max_width(t_setup *setup)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (i < setup->map_height)
	{
		len = ft_strlen(setup->map[i]);
		if (len > setup->map_width)
			setup->map_width = len;
		i++;
	}
}

void	validate_config_complete(t_setup *setup)
{
	if (!setup->textures.north_texture
		|| !setup->textures.south_texture
		|| !setup->textures.west_texture
		|| !setup->textures.east_texture)
		error_exit(ERR_TEXTURE);
	if (setup->floor.r == -1
		|| setup->floor.g == -1
		|| setup->floor.b == -1)
		error_exit(ERR_COLOR);
}

void	normalize_map(t_setup *setup)
{
	int		i;
	int		j;
	int		len;
	char	*new_line;

	validate_config_complete(setup);
	i = 0;
	j = 0;
	find_max_width(setup);
	while (i < setup->map_height)
	{
		len = ft_strlen(setup->map[i]);
		if (len < setup->map_width)
		{
			new_line = malloc(setup->map_width + 1);
			if (!new_line)
				error_exit(ERR_MALLOC);
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
		i++;
	}
}

void	substr_map_line_wo_bn(t_setup *setup, char *line, int map_count)
{
	int	len;
	
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		setup->map[map_count] = ft_substr(line, 0, len - 1);
	else
		setup->map[map_count] = ft_strdup(line);
	if (!setup->map[map_count])
		error_exit(ERR_MALLOC);
}

void write_map(t_setup *setup, char *line, int fd)
{
	int map_count;
	int map_capacity;

	map_capacity = 100;
	map_count = 0;
	setup->map = malloc(sizeof(char *) * map_capacity);
	substr_map_line_wo_bn(setup, line, map_count);
	map_count++;
	free(line);
	line = get_next_line(fd);
	while (line)
	{
		if (map_count >= map_capacity)
		{
			map_capacity *= 2;
			setup->map = ft_realloc_array(setup->map, map_count, 
							sizeof(char *) * map_capacity);
			if (!setup->map)
				error_exit(ERR_MALLOC);
		}
		substr_map_line_wo_bn(setup, line, map_count);
		map_count++;
		free(line);
		line = get_next_line(fd);
	}
	setup->map[map_count] = NULL;
	setup->map_height = map_count;
	normalize_map(setup);
}

void parser(char *filename)
{
	int		fd;
	char	*line;
	int		result;
	t_setup	setup;
	
	fd = validate_file(filename);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
	init_setup(&setup);
	while (line)
	{
		result = parse_config(line, &setup);
		if (result == ERROR_TEXTURE)
			error_exit(ERR_TEXTURE);
		if (result == ERROR_COLOR)
			error_exit(ERR_COLOR);
		if (result == ERROR_PARSE)
			error_exit(ERR_PARSE);
		if (result == MAP_FOUND)
		{
			write_map(&setup, line, fd);
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	print_setup(&setup);
	// validate_map(&setup);
}
