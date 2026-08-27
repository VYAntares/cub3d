/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:38:19 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:38:19 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	move_forward(t_game *game)
{
	game->scene.player.x += game->scene.player.dir_x * game->move_speed;
	game->scene.player.y += game->scene.player.dir_y * game->move_speed;
}

static void	move_backward(t_game *game)
{
	game->scene.player.x -= game->scene.player.dir_x * game->move_speed;
	game->scene.player.y -= game->scene.player.dir_y * game->move_speed;
}

static void	strafe_left(t_game *game)
{
	game->scene.player.x += game->scene.player.dir_y * game->move_speed;
	game->scene.player.y -= game->scene.player.dir_x * game->move_speed;
}

static void	strafe_right(t_game *game)
{
	game->scene.player.x -= game->scene.player.dir_y * game->move_speed;
	game->scene.player.y += game->scene.player.dir_x * game->move_speed;
}

void	move_player(t_game *game)
{
	if (!game)
		return ;
	if (game->input.w)
		move_forward(game);
	if (game->input.s)
		move_backward(game);
	if (game->input.a)
		strafe_left(game);
	if (game->input.d)
		strafe_right(game);
}
