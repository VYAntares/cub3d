/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:38:02 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:38:02 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	load_texture_image(t_game *game, t_texture *texture)
{
	texture->img.img = mlx_xpm_file_to_image(game->mlx.mlx,
			texture->path,
			&texture->img.width,
			&texture->img.height);
	if (!texture->img.img)
		return (error_msg("Failed to load texture"));
	texture->img.addr = mlx_get_data_addr(texture->img.img,
			&texture->img.bpp,
			&texture->img.line_len,
			&texture->img.endian);
	if (!texture->img.addr)
		return (error_msg("Failed to access texture data"));
	return (SUCCESS);
}

int	load_textures(t_game *game)
{
	int	i;

	if (!game || !game->mlx.mlx)
		return (error_msg("Internal error: load_textures"));
	i = 0;
	while (i < 4)
	{
		if (!game->scene.textures[i].path)
			return (error_msg("Missing texture path"));
		if (!load_texture_image(game, &game->scene.textures[i]))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
