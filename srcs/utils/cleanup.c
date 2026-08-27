/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:36:51 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:36:51 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	destroy_img(void *mlx, t_img *img)
{
	if (!mlx || !img || !img->img)
		return ;
	mlx_destroy_image(mlx, img->img);
	img->img = NULL;
	img->addr = NULL;
	img->bpp = 0;
	img->line_len = 0;
	img->endian = 0;
	img->width = 0;
	img->height = 0;
}

static void	destroy_textures(t_game *game)
{
	int	i;

	if (!game || !game->mlx.mlx)
		return ;
	i = 0;
	while (i < 4)
	{
		destroy_img(game->mlx.mlx, &game->scene.textures[i].img);
		i++;
	}
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx.mlx)
	{
		destroy_textures(game);
		destroy_img(game->mlx.mlx, &game->mlx.frame);
		if (game->mlx.win)
		{
			mlx_destroy_window(game->mlx.mlx, game->mlx.win);
			game->mlx.win = NULL;
		}
		mlx_destroy_display(game->mlx.mlx);
		free(game->mlx.mlx);
		game->mlx.mlx = NULL;
	}
	free_scene(&game->scene);
}
