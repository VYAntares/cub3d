/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:40:45 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 16:27:41 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/*
** Écrit un pixel de couleur dans l'image buffer en mémoire.
** 
** MLX ne dessine pas directement à l'écran pixel par pixel, mais utilise un
** buffer d'image en mémoire. Cette fonction modifie directement ce buffer.
**
** Fonctionnement :
** 1. Vérifie que les coordonnées (x,y) sont dans les limites de l'écran
** 2. Calcule l'adresse mémoire exacte du pixel :
**    - y * line_length : saute y lignes complètes
**    - x * (bits_per_pixel / 8) : avance de x pixels dans la ligne
**    - line_length inclut le padding mémoire (alignement)
** 3. Écrit la couleur (format 0xRRGGBB) à cette adresse
**
** Exemple : pixel (100, 50) sur écran 1280x720, 32 bits/pixel
** - Adresse = base + (50 * line_length) + (100 * 4 bytes)
** - Le cast (unsigned int *) permet d'écrire 4 bytes d'un coup
**
** Note : l'image n'apparaît à l'écran qu'après mlx_put_image_to_window().
** Tous les pixels sont d'abord écrits en mémoire, puis affichés en une fois.
*/
void	my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= game->width || y < 0 || y >= game->height)
		return ;
	dst = game->addr + (y * game->line_length + x
			* (game->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

/*
** Récupère la couleur d'un pixel spécifique dans une texture.
** 
** Les textures sont stockées en mémoire comme des tableaux de pixels.
** Cette fonction lit directement la couleur à la position (tex_x, tex_y).
**
** Fonctionnement :
** 1. Vérifie que les coordonnées sont valides (dans les limites de la texture)
**    - Si hors limites : retourne noir (0x000000) pour éviter un crash
** 2. Calcule l'adresse mémoire exacte du pixel dans la texture :
**    - tex_y * line_length : saute tex_y lignes complètes
**    - tex_x * (bits_per_pixel / 8) : avance de tex_x pixels dans la ligne
** 3. Lit et retourne la couleur au format 0xRRGGBB (unsigned int)
**
** Exemple : lire pixel (32, 16) d'une texture 64x64 :
** - Adresse = base_texture + (16 * line_length) + (32 * 4 bytes)
** - Retourne la couleur stockée à cette adresse (ex: 0xFF8040)
**
** Cette fonction est appelée pour chaque pixel du mur affiché à l'écran,
** permettant de "plaquer" la texture sur la surface du mur.
*/
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
