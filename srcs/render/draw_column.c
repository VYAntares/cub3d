/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_column.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:04:29 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:04:29 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_texture	*get_wall_texture(t_game *game, t_ray *ray)
{
	if (ray->side == SIDE_X)
	{
		if (ray->ray_dir_x < 0)
			return (&game->scene.textures[TEX_WE]);
		return (&game->scene.textures[TEX_EA]);
	}
	if (ray->ray_dir_y < 0)
		return (&game->scene.textures[TEX_NO]);
	return (&game->scene.textures[TEX_SO]);
}

static int	get_texture_pixel(t_img *img, int x, int y)
{
	char	*src;

	if (!img || !img->img || !img->addr)
		return (0);
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= img->width)
		x = img->width - 1;
	if (y >= img->height)
		y = img->height - 1;
	src = img->addr + (y * img->line_len + x * (img->bpp / 8));
	return (*(unsigned int *)src);
}

static void	draw_texture_slice(t_game *game, t_ray *ray, int x,
	t_texture *texture)
{
	double	step;
	double	tex_pos;
	int		y;
	int		tex_y;
	int		color;

	step = (double)texture->img.height / (double)ray->line_height;
	tex_pos = (ray->draw_start - game->screen_height / 2
			+ ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)tex_pos;
		color = get_texture_pixel(&texture->img, ray->tex_x, tex_y);
		put_pixel(&game->mlx.frame, x, y, color);
		tex_pos += step;
		y++;
	}
}

void	draw_column(t_game *game, t_ray *ray, int x)
{
	t_texture	*texture;

	if (!game || !ray)
		return ;
	texture = get_wall_texture(game, ray);
	if (!texture->img.img || texture->img.height <= 0)
		return ;
	draw_texture_slice(game, ray, x, texture);
}
