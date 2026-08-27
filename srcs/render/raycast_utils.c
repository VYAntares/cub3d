/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:05:58 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:05:58 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	compute_wall_projection(t_game *game, t_ray *ray)
{
	if (ray->side == SIDE_X)
		ray->perp_dist = ray->side_dist_x - ray->delta_dist_x;
	else
		ray->perp_dist = ray->side_dist_y - ray->delta_dist_y;
	if (ray->perp_dist <= 0.0)
		ray->perp_dist = 0.0001;
	ray->line_height = (int)(game->screen_height / ray->perp_dist);
	ray->draw_start = -ray->line_height / 2 + game->screen_height / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + game->screen_height / 2;
	if (ray->draw_end >= game->screen_height)
		ray->draw_end = game->screen_height - 1;
}

void	compute_tex_x(t_game *game, t_ray *ray, t_texture *texture)
{
	double	wall_x;

	if (ray->side == SIDE_X)
		wall_x = game->scene.player.y + ray->perp_dist * ray->ray_dir_y;
	else
		wall_x = game->scene.player.x + ray->perp_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	ray->tex_x = (int)(wall_x * (double)texture->img.width);
	if (ray->side == SIDE_X && ray->ray_dir_x > 0)
		ray->tex_x = texture->img.width - ray->tex_x - 1;
	if (ray->side == SIDE_Y && ray->ray_dir_y < 0)
		ray->tex_x = texture->img.width - ray->tex_x - 1;
	if (ray->tex_x < 0)
		ray->tex_x = 0;
	if (ray->tex_x >= texture->img.width)
		ray->tex_x = texture->img.width - 1;
}

t_texture	*get_texture_for_ray(t_game *game, t_ray *ray)
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
