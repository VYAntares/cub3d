/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_scene.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:37:43 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:37:43 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	textures_are_present(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!scene->textures[i].path)
			return (error_msg("Missing texture identifier"));
		i++;
	}
	return (SUCCESS);
}

static int	colors_are_present(t_scene *scene)
{
	if (scene->floor.value == -1)
		return (error_msg("Missing floor color"));
	if (scene->ceiling.value == -1)
		return (error_msg("Missing ceiling color"));
	return (SUCCESS);
}

static int	map_is_present(t_scene *scene)
{
	if (!scene->map.grid || scene->map.height == 0)
		return (error_msg("Missing map"));
	return (SUCCESS);
}

int	validate_scene(t_scene *scene)
{
	if (!scene)
		return (error_msg("Internal error: validate_scene"));
	if (!textures_are_present(scene))
		return (FAILURE);
	if (!colors_are_present(scene))
		return (FAILURE);
	if (!map_is_present(scene))
		return (FAILURE);
	if (!validate_map(scene))
		return (FAILURE);
	return (SUCCESS);
}
