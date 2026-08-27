/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:37:58 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:37:58 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_player_direction(t_player *player, char spawn);

static int	set_player_spawn(t_scene *scene, int x, int y)
{
	if (scene->map.grid[y][x] != scene->player.spawn)
		return (0);
	scene->player.x = x + 0.5;
	scene->player.y = y + 0.5;
	set_player_direction(&scene->player, scene->player.spawn);
	scene->map.grid[y][x] = '0';
	return (1);
}

static int	find_spawn_in_row(t_scene *scene, int y)
{
	int	x;

	x = 0;
	while (scene->map.grid[y][x])
	{
		if (set_player_spawn(scene, x, y))
			return (SUCCESS);
		x++;
	}
	return (FAILURE);
}

int	init_player_from_spawn(t_scene *scene)
{
	int	y;

	if (!scene || !scene->map.grid)
		return (error_msg("Internal error: init_player_from_spawn"));
	y = 0;
	while (y < scene->map.height)
	{
		if (find_spawn_in_row(scene, y))
			return (SUCCESS);
		y++;
	}
	return (error_msg("Player spawn not found in map"));
}
