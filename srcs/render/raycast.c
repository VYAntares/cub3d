/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 20:06:13 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 20:06:13 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		compute_wall_projection(t_game *game, t_ray *ray);
void		compute_tex_x(t_game *game, t_ray *ray, t_texture *texture);
t_texture	*get_texture_for_ray(t_game *game, t_ray *ray);

static char	get_map_cell(t_map *map, int y, int x)
{
	int	len;

	if (!map || !map->grid)
		return ('\0');
	if (y < 0 || y >= map->height)
		return ('\0');
	if (!map->grid[y])
		return ('\0');
	len = (int)ft_strlen(map->grid[y]);
	if (x < 0 || x >= len)
		return ('\0');
	return (map->grid[y][x]);
}

static void	init_ray(t_game *game, t_ray *ray, int x)
{
	ray->camera_x = 2.0 * x / (double)game->screen_width - 1.0;
	ray->ray_dir_x = game->scene.player.dir_x
		+ game->scene.player.plane_x * ray->camera_x;
	ray->ray_dir_y = game->scene.player.dir_y
		+ game->scene.player.plane_y * ray->camera_x;
	ray->map_x = (int)game->scene.player.x;
	ray->map_y = (int)game->scene.player.y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);
	ray->hit = 0;
}

static void	init_step_and_side_dist(t_game *game, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->scene.player.x - ray->map_x)
			* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->scene.player.x)
			* ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->scene.player.y - ray->map_y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->scene.player.y)
			* ray->delta_dist_y;
	}
}

static void	perform_dda(t_game *game, t_ray *ray)
{
	char	cell;

	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = SIDE_X;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = SIDE_Y;
		}
		cell = get_map_cell(&game->scene.map, ray->map_y, ray->map_x);
		if (cell == '\0' || cell == ' ' || cell == '1')
			ray->hit = 1;
	}
}

void	raycast_frame(t_game *game)
{
	t_ray		ray;
	t_texture	*texture;
	int			x;

	if (!game)
		return ;
	x = 0;
	while (x < game->screen_width)
	{
		init_ray(game, &ray, x);
		init_step_and_side_dist(game, &ray);
		perform_dda(game, &ray);
		compute_wall_projection(game, &ray);
		texture = get_texture_for_ray(game, &ray);
		compute_tex_x(game, &ray, texture);
		draw_column(game, &ray, x);
		x++;
	}
}
