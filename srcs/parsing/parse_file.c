/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:37:29 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:37:29 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_scene_config_line(const char *line);
int	parse_scene_config_line(const char *line, t_scene *scene);

static char	*strip_newline(char *line)
{
	size_t	len;
	char	*clean;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
		len--;
	clean = ft_substr(line, 0, len);
	free(line);
	if (!clean)
		error_msg("Malloc failed");
	return (clean);
}

static int	process_line(char *line, t_scene *scene, int *in_map)
{
	if (!*in_map)
	{
		if (line_is_empty(line))
			return (SUCCESS);
		if (is_scene_config_line(line))
			return (parse_scene_config_line(line, scene));
		if (line_is_map_start(line))
		{
			*in_map = 1;
			return (parse_map_line(&scene->map, line));
		}
		return (error_msg("Invalid configuration line"));
	}
	if (line_is_empty(line))
		return (error_msg("Empty line inside map"));
	if (!line_is_map_start(line))
		return (error_msg("Invalid content after map start"));
	return (parse_map_line(&scene->map, line));
}

static int	read_file_lines(int fd, t_scene *scene)
{
	char	*line;
	char	*clean;
	int		in_map;

	in_map = 0;
	line = get_next_line(fd);
	while (line)
	{
		clean = strip_newline(line);
		if (!clean || !process_line(clean, scene, &in_map))
		{
			free(clean);
			return (FAILURE);
		}
		free(clean);
		line = get_next_line(fd);
	}
	return (SUCCESS);
}

int	parse_file(const char *filename, t_scene *scene)
{
	int	fd;
	int	status;

	if (!filename || !scene)
		return (error_msg("Internal error: parse_file"));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (error_msg("Could not open map file"));
	status = read_file_lines(fd, scene);
	close(fd);
	return (status);
}
