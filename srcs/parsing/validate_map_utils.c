/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:03:44 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:03:44 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' '
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

char	get_map_char(t_map *map, int y, int x)
{
	int	len;

	if (y < 0 || y >= map->height)
		return ('\0');
	if (!map->grid[y])
		return ('\0');
	len = (int)ft_strlen(map->grid[y]);
	if (x < 0 || x >= len)
		return ('\0');
	return (map->grid[y][x]);
}

int	cell_is_closed(t_map *map, int y, int x)
{
	char	up;
	char	down;
	char	left;
	char	right;

	up = get_map_char(map, y - 1, x);
	down = get_map_char(map, y + 1, x);
	left = get_map_char(map, y, x - 1);
	right = get_map_char(map, y, x + 1);
	if (up == '\0' || down == '\0' || left == '\0' || right == '\0')
		return (error_msg("Map is not closed"));
	if (up == ' ' || down == ' ' || left == ' ' || right == ' ')
		return (error_msg("Map is not closed"));
	return (SUCCESS);
}

int	validate_map_row(t_scene *scene, int y, int *player_count)
{
	int		x;
	char	cell;

	x = 0;
	while (scene->map.grid[y][x])
	{
		cell = scene->map.grid[y][x];
		if (!is_valid_map_char(cell))
			return (error_msg("Invalid character in map"));
		if (is_player_char(cell))
		{
			(*player_count)++;
			scene->player.spawn = cell;
		}
		if ((cell == '0' || is_player_char(cell))
			&& !cell_is_closed(&scene->map, y, x))
			return (FAILURE);
		x++;
	}
	return (SUCCESS);
}
