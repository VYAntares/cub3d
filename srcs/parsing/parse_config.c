/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:56:06 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 16:56:25 by eahmeti          ###   ########.fr       */
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
		return (1);
	}
	return (0);
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
