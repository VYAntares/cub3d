/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:06:42 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/14 21:43:52 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <math.h>

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

void	rotate_left(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	game->player.dir_x = game->player.dir_x * cos(-ROT_SPEED)
		- game->player.dir_y * sin(-ROT_SPEED);
	game->player.dir_y = old_dir_x * sin(-ROT_SPEED)
		+ game->player.dir_y * cos(-ROT_SPEED);
	game->player.plane_x = game->player.plane_x * cos(-ROT_SPEED)
		- game->player.plane_y * sin(-ROT_SPEED);
	game->player.plane_y = old_plane_x * sin(-ROT_SPEED)
		+ game->player.plane_y * cos(-ROT_SPEED);
	render_frame(game);
}

void	rotate_right(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	game->player.dir_x = game->player.dir_x * cos(ROT_SPEED)
		- game->player.dir_y * sin(ROT_SPEED);
	game->player.dir_y = old_dir_x * sin(ROT_SPEED)
		+ game->player.dir_y * cos(ROT_SPEED);
	game->player.plane_x = game->player.plane_x * cos(ROT_SPEED)
		- game->player.plane_y * sin(ROT_SPEED);
	game->player.plane_y = old_plane_x * sin(ROT_SPEED)
		+ game->player.plane_y * cos(ROT_SPEED);
	render_frame(game);
}
