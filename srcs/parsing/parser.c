/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:28:38 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 16:57:11 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
	return (close(fd), clean_up_gnl(fd), validate_map(setup));
}
