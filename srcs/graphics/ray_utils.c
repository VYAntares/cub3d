/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:44:34 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 17:15:25 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
