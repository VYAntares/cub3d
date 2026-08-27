/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:03:56 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:03:56 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_map_row(t_scene *scene, int y, int *player_count);

int	validate_map(t_scene *scene)
{
	int	y;
	int	player_count;

	if (!scene || !scene->map.grid)
		return (error_msg("Internal error: validate_map"));
	y = 0;
	player_count = 0;
	while (y < scene->map.height)
	{
		if (!validate_map_row(scene, y, &player_count))
			return (FAILURE);
		y++;
	}
	if (player_count != 1)
		return (error_msg("Map must contain exactly one player"));
	return (SUCCESS);
}
