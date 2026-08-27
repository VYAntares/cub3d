/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:37:26 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:37:26 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static const char	*skip_spaces(const char *s)
{
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	return (s);
}

static int	parse_uint_component(const char **line, int *value)
{
	int	result;

	*line = skip_spaces(*line);
	if (!ft_isdigit(**line))
		return (FAILURE);
	result = 0;
	while (ft_isdigit(**line))
	{
		result = (result * 10) + (**line - '0');
		(*line)++;
	}
	*value = result;
	return (SUCCESS);
}

static int	parse_rgb_values(const char *line, int rgb[3])
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (!parse_uint_component(&line, &rgb[i]))
			return (FAILURE);
		if (!is_valid_rgb_value(rgb[i]))
			return (FAILURE);
		line = skip_spaces(line);
		if (i < 2)
		{
			if (*line != ',')
				return (FAILURE);
			line++;
		}
		i++;
	}
	line = skip_spaces(line);
	if (*line != '\0')
		return (FAILURE);
	return (SUCCESS);
}

static int	fill_color(t_color *color, int rgb[3])
{
	color->r = rgb[0];
	color->g = rgb[1];
	color->b = rgb[2];
	color->value = rgb_to_int(rgb[0], rgb[1], rgb[2]);
	return (SUCCESS);
}

int	parse_color_line(const char *line, t_scene *scene)
{
	int	rgb[3];

	if (!line || !scene)
		return (error_msg("Internal error: parse_color_line"));
	line = skip_spaces(line);
	if ((*line != 'F' && *line != 'C')
		|| (line[1] != ' ' && !(line[1] >= 9 && line[1] <= 13)))
		return (error_msg("Invalid color identifier"));
	if (*line == 'F' && scene->floor.value != -1)
		return (error_msg("Duplicate floor color"));
	if (*line == 'C' && scene->ceiling.value != -1)
		return (error_msg("Duplicate ceiling color"));
	if (!parse_rgb_values(line + 1, rgb))
		return (error_msg("Invalid RGB color format"));
	if (*line == 'F')
		return (fill_color(&scene->floor, rgb));
	return (fill_color(&scene->ceiling, rgb));
}
