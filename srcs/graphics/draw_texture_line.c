/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:47:04 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 17:15:44 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

/*
** Initialise les paramètres pour le rendu texturé d'un mur.
** 
** ÉTAPE 1 : Sélection de la texture
** ────────────────────────────────────────────────────────────────────
** Choisit la texture appropriée (Nord/Sud/Est/Ouest) selon la face du mur
** touchée via get_texture_index().
**
** ÉTAPE 2 : Calcul de wall_x (position de l'impact)
** ────────────────────────────────────────────────────────────────────
** Calcule la coordonnée exacte où le rayon touche le mur :
** - Mur vertical (side=0) : wall_x = player.y + distance × direction_y
** - Mur horizontal (side=1) : wall_x = player.x + distance × direction_x
**
** ÉTAPE 3 : Conversion en position fractionnaire (0.0 à 1.0)
** ────────────────────────────────────────────────────────────────────
** En soustrayant floor(wall_x), on garde la partie décimale :
** wall_x = 9.65 → 0.65 (65% le long du mur)
**
** ÉTAPE 4 : Conversion en colonne de texture (tex_x)
** ────────────────────────────────────────────────────────────────────
** Transforme la position 0.0-1.0 en index de colonne dans la texture.
** Exemple : wall_x = 0.6, texture 64px → tex_x = 38
**
** ÉTAPE 5 : Inversion (flip) si nécessaire
** ────────────────────────────────────────────────────────────────────
** Inverse la texture pour éviter l'effet miroir dans ces cas :
** - Mur vertical ET rayon va vers la droite (ray_dir_x > 0)
** - Mur horizontal ET rayon va vers le haut (ray_dir_y < 0)
** Formule : tex_x = width - tex_x - 1
**
** Résultat : tr->texture et tr->tex_x prêts pour le rendu du mur.
*/
static void	init_texture_rendering(t_game *game, t_ray *ray, t_tex_render *tr)
{
	tr->texture = &game->textures[get_texture_index(ray)];
	if (ray->side == 0)
		tr->wall_x = game->player.y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		tr->wall_x = game->player.x + ray->perp_wall_dist * ray->ray_dir_x;
	tr->wall_x -= floor(tr->wall_x);
	tr->tex_x = (int)(tr->wall_x * (double)tr->texture->width);
	if ((ray->side == 0 && ray->ray_dir_x > 0)
		|| (ray->side == 1 && ray->ray_dir_y < 0))
		tr->tex_x = tr->texture->width - tr->tex_x - 1;
}

/*
** Calcule les paramètres de mapping vertical de la texture sur le mur.
** 
** ÉTAPE 1 : Calcul du step (incrémentation texture)
** ────────────────────────────────────────────────────────────────────
** Le step détermine de combien on avance dans la texture pour chaque pixel
** écran. Il contrôle l'étirement/compression de la texture selon la distance.
**
** Formule : step = hauteur_texture / hauteur_mur_écran
**
** Exemples (texture 64px) :
** - Mur proche (line_height=640) : step=0.1 → texture très étirée (zoom)
** - Mur loin (line_height=80) : step=0.8 → texture compressée
**
** ÉTAPE 2 : Calcul de tex_pos (position de départ)
** ────────────────────────────────────────────────────────────────────
** tex_pos détermine où commencer dans la texture pour centrer correctement
** le rendu, surtout quand le mur dépasse les limites de l'écran.
**
** Formule : tex_pos = (draw_start - centre + demi_hauteur_mur) × step
**
** Si un mur géant dépasse l'écran (draw_start clampé à 0), cette formule
** calcule automatiquement où débuter dans la texture pour que le rendu
** soit centré et cohérent, même partiellement visible.
**
** Résultat : tr->step et tr->tex_pos prêts pour le rendu colonne par colonne.
*/
static void	calculate_texture_step(t_game *game, t_ray *ray, t_tex_render *tr)
{
	tr->step = 1.0 * tr->texture->height / ray->line_height;
	tr->tex_pos = (ray->draw_start - game->height / 2
			+ ray->line_height / 2) * tr->step;
}

/*
** Dessine une colonne verticale texturée du mur à l'écran.
** 
** Pour chaque pixel Y entre draw_start et draw_end, cette fonction :
** 1. Calcule tex_y (position Y dans la texture)
** 2. Récupère la couleur correspondante dans la texture
** 3. Applique un assombrissement pour les murs Est/Ouest (effet profondeur)
** 4. Dessine le pixel à l'écran
**
** CALCUL DE tex_y
** ────────────────────────────────────────────────────────────────────
** tex_y = (int)tex_pos & (texture->height - 1)
**
** L'opérateur & (bitwise AND) remplace le modulo (%) et est plus rapide.
** Fonctionne uniquement si texture->height est une puissance de 2 (64, 128).
**
** Exemple : texture->height = 64, height - 1 = 63 = 0b00111111 (masque)
**   tex_pos = 15 → tex_y = 15 & 63 = 15 ✓
**   tex_pos = 70 → tex_y = 70 & 63 = 6  ✓ (ramené dans les limites)
**
** ASSOMBRISSEMENT DES MURS EST/OUEST
** ────────────────────────────────────────────────────────────────────
** if (ray->side == 1) → color = (color >> 1) & 0x7F7F7F
**
** Divise chaque composante RGB par 2 pour créer un contraste avec les murs
** Nord/Sud et améliorer la perception de profondeur.
**
** INCRÉMENTATION
** ────────────────────────────────────────────────────────────────────
** tex_pos += step : avance dans la texture selon le step calculé, permettant
** d'étirer ou compresser la texture selon la distance du mur.
*/
static void	render_textured_column(t_game *game,
							t_ray *ray, t_tex_render *tr, int x)
{
	int	y;
	int	tex_y;
	int	color;

	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)tr->tex_pos & (tr->texture->height - 1);
		tr->tex_pos += tr->step;
		color = get_texture_color(tr->texture, tr->tex_x, tex_y);
		if (ray->side == 1)
			color = (color >> 1) & 0x7F7F7F;
		my_mlx_pixel_put(game, x, y, color);
		y++;
	}
}

void	draw_textured_line(t_game *game, t_ray *ray, int x)
{
	t_tex_render	tr;

	init_texture_rendering(game, ray, &tr);
	calculate_texture_step(game, ray, &tr);
	render_textured_column(game, ray, &tr, x);
}
