/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:48:18 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 17:15:13 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	cast_ray(t_game *game, int x)
{
	t_ray	ray;

	init_ray(game, &ray, x);
	calculate_step_and_side_dist(game, &ray);
	perform_dda(game, &ray);
	calculate_distance(game, &ray);
	draw_floor_ceiling(game, x, ray.draw_start, ray.draw_end);
	draw_textured_line(game, &ray, x);
}

/*
** Génère une image complète de la vue du joueur.
** 
** Pour chaque colonne verticale de pixels de l'écran (x de 0 à width-1),
** lance un rayon via cast_ray() qui :
** 1. Calcule la direction du rayon (init_ray)
** 2. Trouve le mur le plus proche (DDA algorithm)
** 3. Dessine le plafond, le mur texturé et le sol
**
** Une fois tous les rayons tracés, affiche l'image buffer à l'écran avec
** mlx_put_image_to_window(). Cette fonction est appelée à chaque frame.
*/
void	render_frame(t_game *game)
{
	int	x;

	x = 0;
	while (x < game->width)
	{
		cast_ray(game, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
