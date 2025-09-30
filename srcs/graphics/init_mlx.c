/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:06:54 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 16:31:40 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (1);
	game->width = WIN_WIDTH;
	game->height = WIN_HEIGHT;
	game->win = mlx_new_window(game->mlx, game->width, game->height, "cub3D");
	if (!game->win)
		return (1);
	game->img = mlx_new_image(game->mlx, game->width, game->height);
	if (!game->img)
		return (1);
	game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
			&game->line_length, &game->endian);
	if (!game->addr)
		return (1);
	return (0);
}

int	load_texture(t_game *game, t_texture *texture, char *path)
{
	texture->img = mlx_xpm_file_to_image(game->mlx, path,
			&texture->width, &texture->height);
	if (!texture->img)
		return (1);
	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
			&texture->line_length, &texture->endian);
	if (!texture->addr)
		return (1);
	return (0);
}

int	load_all_textures(t_game *game)
{
	if (load_texture(game, &game->textures[NORTH_TEX],
			game->setup->textures.north_texture) != 0)
		return (1);
	if (load_texture(game, &game->textures[SOUTH_TEX],
			game->setup->textures.south_texture) != 0)
		return (1);
	if (load_texture(game, &game->textures[EAST_TEX],
			game->setup->textures.east_texture) != 0)
		return (1);
	if (load_texture(game, &game->textures[WEST_TEX],
			game->setup->textures.west_texture) != 0)
		return (1);
	return (0);
}

void	cleanup_textures(t_game *game)
{
	int	i;

	if (!game || !game->mlx)
		return ;
	i = 0;
	while (i < 4)
	{
		if (game->textures[i].img)
			mlx_destroy_image(game->mlx, game->textures[i].img);
		i++;
	}
}

void	cleanup_mlx(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	cleanup_textures(game);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}
