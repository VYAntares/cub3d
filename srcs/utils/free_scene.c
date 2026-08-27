/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:04:10 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:04:10 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	free_map_grid(char **grid)
{
	int	i;

	if (!grid)
		return ;
	i = 0;
	while (grid[i])
	{
		free(grid[i]);
		i++;
	}
	free(grid);
}

static void	free_texture_paths(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		free(scene->textures[i].path);
		scene->textures[i].path = NULL;
		i++;
	}
}

static void	reset_scene_data(t_scene *scene)
{
	scene->map.width = 0;
	scene->map.height = 0;
	scene->floor.r = -1;
	scene->floor.g = -1;
	scene->floor.b = -1;
	scene->floor.value = -1;
	scene->ceiling.r = -1;
	scene->ceiling.g = -1;
	scene->ceiling.b = -1;
	scene->ceiling.value = -1;
	scene->player.x = 0.0;
	scene->player.y = 0.0;
	scene->player.dir_x = 0.0;
	scene->player.dir_y = 0.0;
	scene->player.plane_x = 0.0;
	scene->player.plane_y = 0.0;
	scene->player.spawn = 0;
}

void	free_scene(t_scene *scene)
{
	if (!scene)
		return ;
	free_texture_paths(scene);
	if (scene->map.grid)
	{
		free_map_grid(scene->map.grid);
		scene->map.grid = NULL;
	}
	reset_scene_data(scene);
}
