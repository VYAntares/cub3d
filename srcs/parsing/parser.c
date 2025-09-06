/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:28:38 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/06 15:56:18 by eahmeti          ###   ########.fr       */
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
    char **rgb;
    char *color_str;
    
    color_str = ft_substr(trimmed, 1, (ft_strlen(trimmed) - 1));
    if (!color_str)
        return (ERROR_COLOR);
    rgb = ft_split(color_str, ',');
    free(color_str);
    if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
        return (ERROR_COLOR);
    color->r = ft_atoi(rgb[0]);
    color->g = ft_atoi(rgb[1]);
    color->b = ft_atoi(rgb[2]);
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
			printf("  [%2d] %s\n", i, setup->map[i]);
			i++;
		}
	}

	printf("\n=== END SETUP ===\n");
}

void	parser(char *filename)
{
	int		fd;
	char	*line;
	int		result;
	t_setup	setup;
	
	fd = validate_file(filename);
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
			while(line)
			{
				map_line[map_count] = ft_strdup(line);
				map_count++
			}
		}
		free(line);
		line = get_next_line(fd);
    }
	print_setup(&setup);
}
