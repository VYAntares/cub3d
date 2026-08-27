/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:38:24 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:38:24 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate_left(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;
	double	angle;

	if (!game)
		return ;
	angle = -game->rot_speed;
	old_dir_x = game->scene.player.dir_x;
	game->scene.player.dir_x = game->scene.player.dir_x * cos(angle)
		- game->scene.player.dir_y * sin(angle);
	game->scene.player.dir_y = old_dir_x * sin(angle)
		+ game->scene.player.dir_y * cos(angle);
	old_plane_x = game->scene.player.plane_x;
	game->scene.player.plane_x = game->scene.player.plane_x * cos(angle)
		- game->scene.player.plane_y * sin(angle);
	game->scene.player.plane_y = old_plane_x * sin(angle)
		+ game->scene.player.plane_y * cos(angle);
}

void	rotate_right(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;
	double	angle;

	if (!game)
		return ;
	angle = game->rot_speed;
	old_dir_x = game->scene.player.dir_x;
	game->scene.player.dir_x = game->scene.player.dir_x * cos(angle)
		- game->scene.player.dir_y * sin(angle);
	game->scene.player.dir_y = old_dir_x * sin(angle)
		+ game->scene.player.dir_y * cos(angle);
	old_plane_x = game->scene.player.plane_x;
	game->scene.player.plane_x = game->scene.player.plane_x * cos(angle)
		- game->scene.player.plane_y * sin(angle);
	game->scene.player.plane_y = old_plane_x * sin(angle)
		+ game->scene.player.plane_y * cos(angle);
}
