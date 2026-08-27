/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:02:17 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:02:17 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static const char	*skip_spaces(const char *s)
{
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	return (s);
}

int	line_is_empty(const char *line)
{
	line = skip_spaces(line);
	return (*line == '\0');
}

int	line_is_map_start(const char *line)
{
	line = skip_spaces(line);
	if (*line == '\0')
		return (0);
	if (*line == '1' || *line == '0'
		|| *line == 'N' || *line == 'S'
		|| *line == 'E' || *line == 'W')
		return (1);
	return (0);
}

static char	**append_map_grid(t_map *map, char *copy)
{
	char	**new_grid;
	int		i;

	new_grid = malloc(sizeof(char *) * (map->height + 2));
	if (!new_grid)
		return (NULL);
	i = 0;
	while (i < map->height)
	{
		new_grid[i] = map->grid[i];
		i++;
	}
	new_grid[map->height] = copy;
	new_grid[map->height + 1] = NULL;
	free(map->grid);
	return (new_grid);
}

int	parse_map_line(t_map *map, const char *line)
{
	char	**new_grid;
	char	*copy;
	int		len;

	if (!map || !line)
		return (error_msg("Internal error: parse_map_line"));
	copy = ft_strdup(line);
	if (!copy)
		return (error_msg("Malloc failed"));
	new_grid = append_map_grid(map, copy);
	if (!new_grid)
	{
		free(copy);
		return (error_msg("Malloc failed"));
	}
	map->grid = new_grid;
	len = (int)ft_strlen(copy);
	if (len > map->width)
		map->width = len;
	map->height++;
	return (SUCCESS);
}
