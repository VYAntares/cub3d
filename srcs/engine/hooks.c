/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:38:11 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:38:11 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	key_press(int keycode, t_game *game)
{
	if (!game)
		return (FAILURE);
	if (keycode == XK_Escape)
		close_game(game);
	else if (keycode == XK_w)
		game->input.w = 1;
	else if (keycode == XK_a)
		game->input.a = 1;
	else if (keycode == XK_s)
		game->input.s = 1;
	else if (keycode == XK_d)
		game->input.d = 1;
	else if (keycode == XK_Left)
		game->input.left = 1;
	else if (keycode == XK_Right)
		game->input.right = 1;
	return (SUCCESS);
}

int	key_release(int keycode, t_game *game)
{
	if (!game)
		return (FAILURE);
	if (keycode == XK_w)
		game->input.w = 0;
	else if (keycode == XK_a)
		game->input.a = 0;
	else if (keycode == XK_s)
		game->input.s = 0;
	else if (keycode == XK_d)
		game->input.d = 0;
	else if (keycode == XK_Left)
		game->input.left = 0;
	else if (keycode == XK_Right)
		game->input.right = 0;
	return (SUCCESS);
}

int	init_hooks(t_game *game)
{
	if (!game || !game->mlx.win)
		return (error_msg("Internal error: init_hooks"));
	mlx_hook(game->mlx.win, EVENT_KEY_PRESS, MASK_KEY_PRESS, key_press, game);
	mlx_hook(game->mlx.win, EVENT_KEY_RELEASE, MASK_KEY_RELEASE,
		key_release, game);
	mlx_hook(game->mlx.win, EVENT_DESTROY, MASK_DESTROY, close_game, game);
	return (SUCCESS);
}
