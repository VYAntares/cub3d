/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:02:58 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:02:58 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_scene_config_line(const char *line)
{
	while (*line == ' ' || (*line >= 9 && *line <= 13))
		line++;
	if (!ft_strncmp(line, "NO", 2) || !ft_strncmp(line, "SO", 2)
		|| !ft_strncmp(line, "WE", 2) || !ft_strncmp(line, "EA", 2))
		return (1);
	if (*line == 'F' || *line == 'C')
		return (1);
	return (0);
}

int	parse_scene_config_line(const char *line, t_scene *scene)
{
	while (*line == ' ' || (*line >= 9 && *line <= 13))
		line++;
	if (!ft_strncmp(line, "NO", 2) || !ft_strncmp(line, "SO", 2)
		|| !ft_strncmp(line, "WE", 2) || !ft_strncmp(line, "EA", 2))
		return (parse_texture_line(line, scene));
	if (*line == 'F' || *line == 'C')
		return (parse_color_line(line, scene));
	return (error_msg("Invalid configuration line"));
}
