/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:06:42 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 16:36:55 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_wall(t_game *game, int x, int y)
{
	if (x < 0 || y < 0
		|| x >= game->setup->map_width || y >= game->setup->map_height)
		return (1);
	if (game->setup->map[y][x] == '1')
		return (1);
	return (0);
}

void	move_forward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.dir_x * MOVE_SPEED;
	new_y = game->player.y + game->player.dir_y * MOVE_SPEED;
	if (!is_wall(game, (int)new_x, (int)game->player.y))
		game->player.x = new_x;
	if (!is_wall(game, (int)game->player.x, (int)new_y))
		game->player.y = new_y;
	render_frame(game);
}

void	move_backward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.dir_x * MOVE_SPEED;
	new_y = game->player.y - game->player.dir_y * MOVE_SPEED;
	if (!is_wall(game, (int)new_x, (int)game->player.y))
		game->player.x = new_x;
	if (!is_wall(game, (int)game->player.x, (int)new_y))
		game->player.y = new_y;
	render_frame(game);
}

void	move_left(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x + game->player.dir_y * MOVE_SPEED;
	new_y = game->player.y - game->player.dir_x * MOVE_SPEED;
	if (!is_wall(game, (int)new_x, (int)game->player.y))
		game->player.x = new_x;
	if (!is_wall(game, (int)game->player.x, (int)new_y))
		game->player.y = new_y;
	render_frame(game);
}

void	move_right(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.x - game->player.dir_y * MOVE_SPEED;
	new_y = game->player.y + game->player.dir_x * MOVE_SPEED;
	if (!is_wall(game, (int)new_x, (int)game->player.y))
		game->player.x = new_x;
	if (!is_wall(game, (int)game->player.x, (int)new_y))
		game->player.y = new_y;
	render_frame(game);
}
