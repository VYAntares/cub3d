/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:37:55 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:37:55 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	init_framebuffer(t_game *game)
{
	game->mlx.frame.img = mlx_new_image(game->mlx.mlx,
			game->screen_width, game->screen_height);
	if (!game->mlx.frame.img)
		return (error_msg("Failed to create framebuffer"));
	game->mlx.frame.addr = mlx_get_data_addr(game->mlx.frame.img,
			&game->mlx.frame.bpp,
			&game->mlx.frame.line_len,
			&game->mlx.frame.endian);
	if (!game->mlx.frame.addr)
		return (error_msg("Failed to access framebuffer data"));
	game->mlx.frame.width = game->screen_width;
	game->mlx.frame.height = game->screen_height;
	return (SUCCESS);
}

int	init_mlx(t_game *game)
{
	if (!game)
		return (error_msg("Internal error: init_mlx"));
	game->mlx.mlx = mlx_init();
	if (!game->mlx.mlx)
		return (error_msg("Failed to initialize MLX"));
	game->mlx.win = mlx_new_window(game->mlx.mlx,
			game->screen_width,
			game->screen_height,
			WINDOW_TITLE);
	if (!game->mlx.win)
		return (error_msg("Failed to create window"));
	if (!init_framebuffer(game))
		return (FAILURE);
	return (SUCCESS);
}
