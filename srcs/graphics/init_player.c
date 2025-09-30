/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:06:49 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 16:32:11 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	assign_position_north(t_player *player)
{
	player->dir_x = 0.0;
	player->dir_y = -1.0;
	player->plane_x = 0.66;
	player->plane_y = 0.0;
}

void	assign_position_south(t_player *player)
{
	player->dir_x = 0.0;
	player->dir_y = 1.0;
	player->plane_x = -0.66;
	player->plane_y = 0.0;
}

void	assign_position_east(t_player *player)
{
	player->dir_x = 1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.66;
}

void	assign_position_west(t_player *player)
{
	player->dir_x = -1.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = -0.66;
}

void	init_player(t_player *player, t_setup *setup)
{
	player->x = (double)setup->spawn_x + 0.5;
	player->y = (double)setup->spawn_y + 0.5;
	if (setup->spawn_dir == 'N')
		assign_position_north(player);
	else if (setup->spawn_dir == 'S')
		assign_position_south(player);
	else if (setup->spawn_dir == 'E')
		assign_position_east(player);
	else if (setup->spawn_dir == 'W')
		assign_position_west(player);
}
