/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:37:33 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:37:33 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static const char	*skip_spaces(const char *s)
{
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	return (s);
}

static int	get_texture_id(const char *line)
{
	if (!ft_strncmp(line, "NO", 2))
		return (TEX_NO);
	if (!ft_strncmp(line, "SO", 2))
		return (TEX_SO);
	if (!ft_strncmp(line, "WE", 2))
		return (TEX_WE);
	if (!ft_strncmp(line, "EA", 2))
		return (TEX_EA);
	return (-1);
}

static char	*extract_path(const char *line)
{
	const char	*start;
	const char	*end;

	start = skip_spaces(line + 2);
	if (*start == '\0')
		return (NULL);
	end = start + ft_strlen(start);
	while (end > start && (end[-1] == ' ' || (end[-1] >= 9 && end[-1] <= 13)))
		end--;
	if (end <= start)
		return (NULL);
	return (ft_substr(start, 0, end - start));
}

int	parse_texture_line(const char *line, t_scene *scene)
{
	int		tex_id;
	char	*path;

	if (!line || !scene)
		return (error_msg("Internal error: parse_texture_line"));
	line = skip_spaces(line);
	tex_id = get_texture_id(line);
	if (tex_id < 0)
		return (error_msg("Invalid texture identifier"));
	if (line[2] != ' ' && !(line[2] >= 9 && line[2] <= 13))
		return (error_msg("Invalid texture line format"));
	if (scene->textures[tex_id].path != NULL)
		return (error_msg("Duplicate texture identifier"));
	path = extract_path(line);
	if (!path)
		return (error_msg("Missing texture path"));
	scene->textures[tex_id].path = path;
	return (SUCCESS);
}
