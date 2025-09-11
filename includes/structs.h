/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:53:45 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 19:25:08 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

// Constantes de fenêtre et textures
# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define TEX_WIDTH 64
# define TEX_HEIGHT 64

// Constantes de mouvement
# define MOVE_SPEED 0.05
# define ROT_SPEED 0.03

// Indices des textures
# define NORTH_TEX  0
# define SOUTH_TEX  1
# define EAST_TEX   2
# define WEST_TEX   3

// Constantes de parsing
# define MAP_FOUND 2

// Structure pour les couleurs RGB
typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;

// Structure pour les chemins des textures (parsing)
typedef struct s_textures
{
    char *north_texture;
    char *south_texture;
    char *west_texture;
    char *east_texture;
} t_textures;

typedef struct s_setup
{
    t_textures  textures;       // Chemins des textures
    t_color     floor;          // Couleur du sol
    t_color     ceiling;        // Couleur du plafond
    char        **map;          // Carte 2D
    int         map_height;     // Hauteur de la carte
    int         map_width;      // Largeur de la carte
    int         spawn_x;        // Position spawn X
    int         spawn_y;        // Position spawn Y
    char        spawn_dir;      // Direction spawn (N,S,E,W)
} t_setup;

// Structure pour les textures MLX (runtime)
typedef struct s_texture
{
    void    *img;               // Image MLX
    char    *addr;              // Adresse des données
    int     width;              // Largeur texture
    int     height;             // Hauteur texture
    int     bits_per_pixel;     // Bits par pixel
    int     line_length;        // Longueur ligne
    int     endian;             // Endianness
} t_texture;

// Structure du joueur
typedef struct s_player
{
    double  x;                  // Position X du joueur
    double  y;                  // Position Y du joueur
    double  dir_x;              // Direction X du regard
    double  dir_y;              // Direction Y du regard
    double  plane_x;            // Plan caméra X (perpendiculaire à dir)
    double  plane_y;            // Plan caméra Y (perpendiculaire à dir)
} t_player;

// Structure principale du jeu
typedef struct s_game
{
    void        *mlx;           // Pointeur MLX
    void        *win;           // Fenêtre
    void        *img;           // Image buffer
    char        *addr;          // Adresse de l'image
    int         bits_per_pixel; // Bits par pixel
    int         line_length;    // Longueur ligne
    int         endian;         // Endianness
    int         width;          // Largeur fenêtre
    int         height;         // Hauteur fenêtre
    t_setup     *setup;         // Configuration parsée
    t_player    player;         // Joueur
    t_texture   textures[4];    // 4 textures (N,S,E,W)
} t_game;

#endif