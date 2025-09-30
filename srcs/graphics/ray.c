/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:40:45 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 16:48:50 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

/*
** Calcule la distance au mur et détermine les limites de rendu à l'écran.
** 
** Cette fonction transforme une distance 3D en coordonnées 2D de pixels,
** en appliquant les règles de la perspective.
**
** ÉTAPE 1 : Calcul de la distance perpendiculaire (perp_wall_dist)
** ────────────────────────────────────────────────────────────────────
** Problème : si on utilise la distance réelle du rayon, on obtient un effet
** fish-eye (distorsion). Il faut la distance perpendiculaire au plan caméra.
**
** Pour un mur vertical (side == 0) :
**   - ray->map_x - player.x : distance horizontale joueur → case touchée
**   - (1 - step_x) / 2 : correction pour pointer vers le bon bord de case
**     * step_x = 1 (droite) → 0 (bord gauche de la case)
**     * step_x = -1 (gauche) → 1 (bord droit de la case)
**   - / ray_dir_x : convertit distance horizontale en distance de rayon
**
** Exemple : joueur en (5.3, 10.0), mur en x=8, ray_dir_x=0.8, step_x=1
**   perp_wall_dist = (8 - 5.3 + 0) / 0.8 = 2.7 / 0.8 = 3.375 unités
**
** Pour un mur horizontal (side == 1) : même logique avec coordonnées Y.
**
** ÉTAPE 2 : Hauteur du mur à l'écran (line_height)
** ────────────────────────────────────────────────────────────────────
** Principe de perspective : plus proche = plus grand, plus loin = plus petit
**   line_height = hauteur_écran / distance
**
** Exemples (écran 720px) :
**   - dist = 0.5 → 720/0.5 = 1440px (énorme, dépasse l'écran)
**   - dist = 1.0 → 720/1.0 = 720px (remplit l'écran)
**   - dist = 2.0 → 720/2.0 = 360px (moitié de l'écran)
**   - dist = 10.0 → 720/10.0 = 72px (petite barre)
**
** ÉTAPE 3 : Pixel de début du mur (draw_start)
** ────────────────────────────────────────────────────────────────────
** Centre le mur verticalement sur l'écran :
**   draw_start = centre_écran - demi_hauteur_mur
**              = height/2 - line_height/2
**
** Exemple : mur 400px de haut, écran 720px
**   draw_start = 360 - 200 = 160 (commence 160px sous le haut)
**
** Le if clamp à 0 si le mur dépasse le haut (murs très proches).
**
** ÉTAPE 4 : Pixel de fin du mur (draw_end)
** ────────────────────────────────────────────────────────────────────
** Même logique pour le bas :
**   draw_end = centre_écran + demi_hauteur_mur
**            = height/2 + line_height/2
**
** Le if clamp à height-1 si le mur dépasse le bas de l'écran.
**
** Résultat : draw_start et draw_end définissent où dessiner le mur,
** le reste étant rempli par le plafond (0 à draw_start-1) et le sol
** (draw_end+1 à height-1).
*/
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
** Dessine le plafond et le sol pour une colonne verticale de pixels.
** 
** Cette fonction remplit les espaces vides au-dessus et en-dessous du mur
** avec les couleurs définies dans la configuration (.cub file).
**
** Conversion des couleurs RGB en format hexadécimal 0xRRGGBB :
** - Décalage binaire << pour positionner chaque composante :
**   * R << 16 : place Rouge dans les bits 16-23    (0xRR0000)
**   * G << 8  : place Vert dans les bits 8-15      (0x00GG00)
**   * B       : reste dans les bits 0-7            (0x0000BB)
** - Opérateur OR | pour combiner les trois :
**   Exemple : R=255, G=128, B=64 → 0xFF0000 | 0x008000 | 0x000040 = 0xFF8040
**
** Structure de rendu vertical pour la colonne x :
** - y=0 à draw_start-1        : plafond (tout ce qui est au-dessus du mur)
** - draw_start à draw_end     : mur (géré par draw_textured_line)
** - draw_end+1 à game->height : sol (tout ce qui est en-dessous du mur)
**
** Note : draw_start et draw_end sont calculés dans calculate_distance() selon
** la distance perpendiculaire au mur (plus proche = mur plus haut à l'écran).
*/
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
