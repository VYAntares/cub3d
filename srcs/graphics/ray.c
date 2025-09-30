/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:40:45 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/25 02:30:26 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= game->width || y < 0 || y >= game->height)
		return ;
	dst = game->addr + (y * game->line_length + x
			* (game->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	get_texture_color(t_texture *texture, int tex_x, int tex_y)
{
	char	*pixel;

	if (tex_x < 0 || tex_x >= texture->width
		|| tex_y < 0 || tex_y >= texture->height)
		return (0x000000);
	pixel = texture->addr + (tex_y * texture->line_length + tex_x
			* (texture->bits_per_pixel / 8));
	return (*(unsigned int *)pixel);
}

/*
** Initialise un rayon pour le pixel x de l'écran.
** Un rayon est une ligne mathématique invisible qui part du joueur vers un
** pixel spécifique de l'écran pour déterminer ce qu'il voit à cet endroit.
**
** Étapes de l'initialisation:
** 1. Convertit la position pixel (0 à WIDTH-1) en coordonnée caméra (-1 à +1)
**    - x=0 (gauche) → camera_x=-1 
**    - x=WIDTH/2 (centre) → camera_x=0 
**    - x=WIDTH-1 (droite) → camera_x≈+1
**
** 2. Calcule la direction exacte du rayon vers ce pixel:
**    - dir = direction où regarde le joueur (centre écran)
**    - plane = vecteur perpendiculaire qui définit le champ de vision
**    - camera_x "décale" le rayon vers la gauche/droite selon le pixel
**
** 3. Détermine la case de grille de départ
**    (position joueur en coordonnées entières)
**
** 4. PRÉCALCULE les delta_dist pour optimiser l'algorithme DDA:
**    - delta_dist_x = long. de rayon nécess. pour traverser 1 case en largeur
**    - delta_dist_y = long. de rayon nécess. pour traverser 1 case en hauteur
**    - Exemple: si ray_dir_x = 0.5 → delta_dist_x = 2.0 (2 unités pour 1 case)
**    - Si direction = 0 (rayon parfaite. perpendic.), on met une valeur infini
**    - fabs() garantit une distance toujours positive peu importe la direction
**
** 5. Initialise le flag hit à 0 (aucun mur touché)
**
** Ces valeurs précalculées seront utilisées dans l'algorithme DDA pour avancer
** case par case le long du rayon jusqu'à toucher un mur, en évitant les calcul
** coûteux (sqrt, divisions) à chaque étape.
*/
void	init_ray(t_game *game, t_ray *ray, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)game->width - 1;
	ray->ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
	ray->ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;
	ray->map_x = (int)game->player.x;
	ray->map_y = (int)game->player.y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->hit = 0;
}

/*
** Calcule la direction de progression et les distances initiales pour le DDA.
** 
** Cette fonction détermine :
** 1. step_x/step_y : direction d'avancement case par case (-1 ou +1)
** 2. side_dist_x/side_dist_y : distance jusqu'aux premières frontières
**
** Le problème : le joueur n'est jamais exactement sur une frontière de case,
** il peut être n'importe où à l'intérieur (ex: position 5.3 dans la case 5).
** 
** Pour side_dist_x :
** - Si rayon va à DROITE : distance = (prochaine_ligne - position_joueur)
**   Exemple : joueur en 5.3 → distance = (6.0 - 5.3) = 0.7 jusqu'à x=6
** - Si rayon va à GAUCHE : distance = (position_joueur - ligne_actuelle)  
**   Exemple : joueur en 5.3 → distance = (5.3 - 5.0) = 0.3 jusqu'à x=5
**
** Cette distance fractionnelle est multipliée par delta_dist pour obtenir
** la vraie distance de rayon jusqu'à la première intersection.
**
** Après ce "premier pas", l'algorithme DDA pourra utiliser des incréments
** réguliers (delta_dist complets) pour avancer de case en case.
*/
void	calculate_step_and_side_dist(t_game *game, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.x - ray->map_x)
			* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player.x)
			* ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.y - ray->map_y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player.y)
			* ray->delta_dist_y;
	}
}

/*
** Algorithme DDA : avancement case par case jusqu'à collision avec un mur.
** 
** Le principe : à chaque étape, choisir la prochaine frontière la plus proche
** (verticale ou horizontale) et avancer jusqu'à elle.
**
** Comparaison side_dist_x < side_dist_y :
** - side_dist_x = distance pour atteindre la prochaine ligne verticale
** - side_dist_y = distance pour atteindre la prochaine ligne horizontale
** - On choisit toujours la plus petite distance (intersection la plus proche)
**
** Si side_dist_x plus petit :
** - On traverse une frontière verticale (changement de case en X)
** - ray->side = 0 (mur Nord/Sud touché si collision)
** - On met à jour side_dist_x pour pointer vers la frontière suivante
**
** Si side_dist_y plus petit :
** - On traverse une frontière horizontale (changement de case en Y) 
** - ray->side = 1 (mur Est/Ouest touché si collision)
** - On met à jour side_dist_y pour pointer vers la frontière suivante
**
** ray->side sert ensuite à déterminer quelle texture afficher et l'orientation
*/
void	perform_dda(t_game *game, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (game->setup->map[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

void	calculate_distance(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player.x
				+ (1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player.y
				+ (1 - ray->step_y) / 2) / ray->ray_dir_y;
	ray->line_height = (int)(game->height / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + game->height / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + game->height / 2;
	if (ray->draw_end >= game->height)
		ray->draw_end = game->height - 1;
}

/*
** Détermine quelle texture utiliser selon la face du mur touchée.
** 
** Le principe : selon la direction d'approche du rayon, on touche une face
** différente du cube, donc on applique la texture correspondante.
**
** Si ray->side == 0 (mur vertical, faces Nord/Sud) :
** - Si step_x > 0 : rayon va vers la droite, touche face OUEST → texture WEST
** - Si step_x < 0 : rayon va vers la gauche, touche face EST → texture EAST
**
** Si ray->side == 1 (mur horizontal, faces Est/Ouest) :
** - Si step_y > 0 : rayon va vers le bas, touche face NORD → texture NORTH  
** - Si step_y < 0 : rayon va vers le haut, touche face SUD → texture SOUTH
**
** Exemple : rayon allant vers le Nord-Est touchera soit la face Sud d'un mur
** (texture SOUTH) soit la face Ouest d'un mur (texture WEST).
*/
int	get_texture_index(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (WEST_TEX);
		else
			return (EAST_TEX);
	}
	else
	{
		if (ray->step_y > 0)
			return (NORTH_TEX);
		else
			return (SOUTH_TEX);
	}
}

void	draw_textured_line(t_game *game, t_ray *ray, int x)
{
	t_texture	*texture;
	double		wall_x;
	int			tex_x;
	int			tex_y;
	double		step;
	double		tex_pos;
	int			y;
	int			color;
	
	// Choisir la texture
	texture = &game->textures[get_texture_index(ray)];
	
	// Calculer wall_x (position exacte où le mur a été touché)
	if (ray->side == 0)
		wall_x = game->player.y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = game->player.x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	
	// x coordinate sur la texture
	tex_x = (int)(wall_x * (double)texture->width);
	if ((ray->side == 0 && ray->ray_dir_x > 0) || (ray->side == 1 && ray->ray_dir_y < 0))
		tex_x = texture->width - tex_x - 1;
	
	// Calculer le step et la position texture initiale
	step = 1.0 * texture->height / ray->line_height;
	tex_pos = (ray->draw_start - game->height / 2 + ray->line_height / 2) * step;
	
	// Dessiner la ligne texturée
	for (y = ray->draw_start; y <= ray->draw_end; y++)
	{
		tex_y = (int)tex_pos & (texture->height - 1);
		tex_pos += step;
		color = get_texture_color(texture, tex_x, tex_y);
		
		// Assombrir les côtés EW pour l'effet de profondeur
		if (ray->side == 1)
			color = (color >> 1) & 0x7F7F7F;
		
		my_mlx_pixel_put(game, x, y, color);
	}
}

void	draw_floor_ceiling(t_game *game, int x, int draw_start, int draw_end)
{
	int	y;
	int	floor_color;
	int	ceiling_color;
	
	floor_color = (game->setup->floor.r << 16)
		| (game->setup->floor.g << 8) | game->setup->floor.b;
	ceiling_color = (game->setup->ceiling.r << 16)
		| (game->setup->ceiling.g << 8) | game->setup->ceiling.b;
	y = 0;
	while (y < draw_start)
	{
		my_mlx_pixel_put(game, x, y, ceiling_color);
		y++;
	}
	y = draw_end + 1;
	while (y < game->height)
	{
		my_mlx_pixel_put(game, x, y, floor_color);
		y++;
	}
}

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
