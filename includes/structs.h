/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:09:11 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 17:18:31 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
# define TEX_WIDTH 64
# define TEX_HEIGHT 64

# define MOVE_SPEED 0.12
# define ROT_SPEED 0.04

# define NORTH_TEX  0
# define SOUTH_TEX  1
# define EAST_TEX   2
# define WEST_TEX   3

# define MAP_FOUND 2

/*
** Membres :
** - r : Composante rouge (0-255)
** - g : Composante verte (0-255)
** - b : Composante bleue (0-255)
*/
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

/*
** Membres :
** - north_texture : Chemin vers la texture Nord
** - south_texture : Chemin vers la texture Sud
** - west_texture  : Chemin vers la texture Ouest
** - east_texture  : Chemin vers la texture Est
*/
typedef struct s_textures
{
	char	*north_texture;
	char	*south_texture;
	char	*west_texture;
	char	*east_texture;
}	t_textures;

/*
** Membres :
** - textures   : Structure contenant les chemins des textures
** - floor      : Couleur du sol
** - ceiling    : Couleur du plafond
** - map        : Carte 2D (tableau de chaînes)
** - map_height : Nombre de lignes de la carte
** - map_width  : Largeur maximale de la carte
** - spawn_x    : Position X de départ du joueur
** - spawn_y    : Position Y de départ du joueur
** - spawn_dir  : Direction initiale ('N', 'S', 'E', 'W')
*/
typedef struct s_setup
{
	t_textures	textures;
	t_color		floor;
	t_color		ceiling;
	char		**map;
	int			map_height;
	int			map_width;
	int			spawn_x;
	int			spawn_y;
	char		spawn_dir;
}	t_setup;

/*
** Membres :
** - img            : Pointeur vers l'image MLX
** - addr           : Adresse des données de l'image
** - width          : Largeur de la texture
** - height         : Hauteur de la texture
** - bits_per_pixel : Bits par pixel
** - line_length    : Longueur d'une ligne en bytes
** - endian         : Ordre des bytes
*/
typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_texture;

/*
** Membres :
** - texture : Pointeur vers la texture à utiliser
** - wall_x  : Position horizontale sur le mur (0.0 à 1.0)
** - tex_x   : Colonne de la texture
** - step    : Pas d'avancement dans la texture
** - tex_pos : Position courante dans la texture
*/
typedef struct s_tex_render
{
	t_texture	*texture;
	double		wall_x;
	int			tex_x;
	double		step;
	double		tex_pos;
}	t_tex_render;

/*
** Membres :
** - x       : Position X du joueur
** - y       : Position Y du joueur
** - dir_x   : Direction X du regard
** - dir_y   : Direction Y du regard
** - plane_x : Plan caméra X (perpendiculaire à dir)
** - plane_y : Plan caméra Y (perpendiculaire à dir)
*/
typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

/*
** Membres :
** - mlx            : Pointeur MLX
** - win            : Fenêtre MLX
** - img            : Image buffer
** - addr           : Adresse de l'image
** - bits_per_pixel : Bits par pixel
** - line_length    : Longueur d'une ligne
** - endian         : Ordre des bytes
** - width          : Largeur de la fenêtre
** - height         : Hauteur de la fenêtre
** - setup          : Configuration parsée
** - player         : Structure du joueur
** - textures       : Tableau des 4 textures (N,S,E,W)
*/
typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
	t_setup		*setup;
	t_player	player;
	t_texture	textures[4];
}	t_game;

/*
** Membres :
** - ray_dir_x      : Direction X du rayon
** - ray_dir_y      : Direction Y du rayon
** - map_x          : Case X actuelle
** - map_y          : Case Y actuelle
** - delta_dist_x   : Distance entre intersections X
** - delta_dist_y   : Distance entre intersections Y
** - side_dist_x    : Distance jusqu'à prochaine intersection X
** - side_dist_y    : Distance jusqu'à prochaine intersection Y
** - step_x         : Direction du pas (-1 ou 1)
** - step_y         : Direction du pas (-1 ou 1)
** - hit            : Mur touché (0 ou 1)
** - side           : Côté du mur (0=NS, 1=EW)
** - perp_wall_dist : Distance perpendiculaire au mur
** - line_height    : Hauteur de la ligne à dessiner
** - draw_start     : Pixel de début
** - draw_end       : Pixel de fin
*/
typedef struct s_ray
{
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_ray;

#endif
