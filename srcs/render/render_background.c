/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:04:46 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:04:46 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	fill_row(t_img *img, int width, int y, int color)
{
	int	x;

	x = 0;
	while (x < width)
	{
		put_pixel(img, x, y, color);
		x++;
	}
}

static void	render_part(t_game *game, int start, int end, int color)
{
	int	y;

	y = start;
	while (y < end)
	{
		fill_row(&game->mlx.frame, game->screen_width, y, color);
		y++;
	}
}

void	render_background(t_game *game)
{
	int	half_height;

	if (!game || !game->mlx.frame.img)
		return ;
	half_height = game->screen_height / 2;
	render_part(game, 0, half_height, game->scene.ceiling.value);
	render_part(game, half_height, game->screen_height,
		game->scene.floor.value);
}
