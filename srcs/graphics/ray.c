/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:40:45 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 19:44:01 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= game->width || y < 0 || y >= game->height)
        return;
    
    dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int get_texture_color(t_texture *texture, int tex_x, int tex_y)
{
    char *pixel;
    
    if (tex_x < 0 || tex_x >= texture->width || tex_y < 0 || tex_y >= texture->height)
        return 0x000000; // Noir par défaut
    
    pixel = texture->addr + (tex_y * texture->line_length + tex_x * (texture->bits_per_pixel / 8));
    return *(unsigned int*)pixel;
}

void init_ray(t_game *game, t_ray *ray, int x)
{
    double camera_x;
    
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

void calculate_step_and_side_dist(t_game *game, t_ray *ray)
{
    if (ray->ray_dir_x < 0)
    {
        ray->step_x = -1;
        ray->side_dist_x = (game->player.x - ray->map_x) * ray->delta_dist_x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_dist_x = (ray->map_x + 1.0 - game->player.x) * ray->delta_dist_x;
    }
    
    if (ray->ray_dir_y < 0)
    {
        ray->step_y = -1;
        ray->side_dist_y = (game->player.y - ray->map_y) * ray->delta_dist_y;
    }
    else
    {
        ray->step_y = 1;
        ray->side_dist_y = (ray->map_y + 1.0 - game->player.y) * ray->delta_dist_y;
    }
}

void perform_dda(t_game *game, t_ray *ray)
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
        
        if (ray->map_x < 0 || ray->map_x >= game->setup->map_width ||
            ray->map_y < 0 || ray->map_y >= game->setup->map_height ||
            game->setup->map[ray->map_y][ray->map_x] == '1')
        {
            ray->hit = 1;
        }
    }
}

void calculate_distance(t_game *game, t_ray *ray)
{
    if (ray->side == 0)
        ray->perp_wall_dist = (ray->map_x - game->player.x + (1 - ray->step_x) / 2) / ray->ray_dir_x;
    else
        ray->perp_wall_dist = (ray->map_y - game->player.y + (1 - ray->step_y) / 2) / ray->ray_dir_y;
    
    ray->line_height = (int)(game->height / ray->perp_wall_dist);
    
    ray->draw_start = -ray->line_height / 2 + game->height / 2;
    if (ray->draw_start < 0)
        ray->draw_start = 0;
    
    ray->draw_end = ray->line_height / 2 + game->height / 2;
    if (ray->draw_end >= game->height)
        ray->draw_end = game->height - 1;
}

int get_texture_index(t_ray *ray)
{
    if (ray->side == 0) // Mur NS
    {
        if (ray->step_x > 0)
            return WEST_TEX;  // Regardant vers l'est, mur ouest
        else
            return EAST_TEX;  // Regardant vers l'ouest, mur est
    }
    else // Mur EW
    {
        if (ray->step_y > 0)
            return NORTH_TEX; // Regardant vers le sud, mur nord
        else
            return SOUTH_TEX; // Regardant vers le nord, mur sud
    }
}

void draw_textured_line(t_game *game, t_ray *ray, int x)
{
    t_texture *texture;
    double wall_x;
    int tex_x, tex_y;
    double step, tex_pos;
    int y, color;
    
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

void draw_floor_ceiling(t_game *game, int x, int draw_start, int draw_end)
{
    int y;
    int floor_color, ceiling_color;
    
    floor_color = (game->setup->floor.r << 16) | 
                  (game->setup->floor.g << 8) | 
                  game->setup->floor.b;
    
    ceiling_color = (game->setup->ceiling.r << 16) | 
                    (game->setup->ceiling.g << 8) | 
                    game->setup->ceiling.b;
    
    // Dessiner le plafond
    for (y = 0; y < draw_start; y++)
        my_mlx_pixel_put(game, x, y, ceiling_color);
    
    // Dessiner le sol
    for (y = draw_end + 1; y < game->height; y++)
        my_mlx_pixel_put(game, x, y, floor_color);
}

void cast_ray(t_game *game, int x)
{
    t_ray ray;
    
    init_ray(game, &ray, x);
    calculate_step_and_side_dist(game, &ray);
    perform_dda(game, &ray);
    calculate_distance(game, &ray);
    
    draw_floor_ceiling(game, x, ray.draw_start, ray.draw_end);
    draw_textured_line(game, &ray, x);
}

void render_frame(t_game *game)
{
    int x;
    
    x = 0;
    while (x < game->width)
    {
        cast_ray(game, x);
        x++;
    }
    
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}