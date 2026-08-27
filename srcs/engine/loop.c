/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:38:15 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:38:15 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	game_loop(t_game *game)
{
	if (!game)
		return (FAILURE);
	move_player(game);
	if (game->input.left)
		rotate_left(game);
	if (game->input.right)
		rotate_right(game);
	render_frame(game);
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win,
		game->mlx.frame.img, 0, 0);
	return (SUCCESS);
}
