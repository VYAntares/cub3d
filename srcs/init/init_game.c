/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:37:49 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:37:49 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_img(t_img *img)
{
	img->img = NULL;
	img->addr = NULL;
	img->bpp = 0;
	img->line_len = 0;
	img->endian = 0;
	img->width = 0;
	img->height = 0;
}

static void	init_texture(t_texture *texture)
{
	texture->path = NULL;
	init_img(&texture->img);
}

static void	init_color(t_color *color)
{
	color->r = -1;
	color->g = -1;
	color->b = -1;
	color->value = -1;
}

static void	init_scene(t_scene *scene)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		init_texture(&scene->textures[i]);
		i++;
	}
	init_color(&scene->floor);
	init_color(&scene->ceiling);
	scene->map.grid = NULL;
	scene->map.width = 0;
	scene->map.height = 0;
	scene->player.x = 0.0;
	scene->player.y = 0.0;
	scene->player.dir_x = 0.0;
	scene->player.dir_y = 0.0;
	scene->player.plane_x = 0.0;
	scene->player.plane_y = 0.0;
	scene->player.spawn = 0;
}

void	init_game(t_game *game)
{
	init_scene(&game->scene);
	game->mlx.mlx = NULL;
	game->mlx.win = NULL;
	init_img(&game->mlx.frame);
	game->input.w = 0;
	game->input.a = 0;
	game->input.s = 0;
	game->input.d = 0;
	game->input.left = 0;
	game->input.right = 0;
	game->screen_width = SCREEN_WIDTH;
	game->screen_height = SCREEN_HEIGHT;
	game->move_speed = MOVE_SPEED;
	game->rot_speed = ROT_SPEED;
}
