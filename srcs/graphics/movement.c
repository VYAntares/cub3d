#include "../../includes/cub3d.h"
#include <math.h>

int is_wall(t_game *game, int x, int y)
{
    if (x < 0 || y < 0 || x >= game->setup->map_width || y >= game->setup->map_height)
        return (1);  // Hors limites = mur
    
    if (game->setup->map[y][x] == '1')
        return (1);  // Case mur
    
    return (0);  // Case libre
}

void move_forward(t_game *game)
{
    double new_x, new_y;
    
    new_x = game->player.x + game->player.dir_x * MOVE_SPEED;
    new_y = game->player.y + game->player.dir_y * MOVE_SPEED;
    
    // Vérifier collision X
    if (!is_wall(game, (int)new_x, (int)game->player.y))
        game->player.x = new_x;
    
    // Vérifier collision Y
    if (!is_wall(game, (int)game->player.x, (int)new_y))
        game->player.y = new_y;
    
    printf("Player moved to: (%.2f, %.2f)\n", game->player.x, game->player.y);
}

void move_backward(t_game *game)
{
    double new_x, new_y;
    
    new_x = game->player.x - game->player.dir_x * MOVE_SPEED;
    new_y = game->player.y - game->player.dir_y * MOVE_SPEED;
    
    // Vérifier collision X
    if (!is_wall(game, (int)new_x, (int)game->player.y))
        game->player.x = new_x;
    
    // Vérifier collision Y
    if (!is_wall(game, (int)game->player.x, (int)new_y))
        game->player.y = new_y;
    
    printf("Player moved to: (%.2f, %.2f)\n", game->player.x, game->player.y);
}

void move_left(t_game *game)
{
    double new_x, new_y;
    
    // Se déplacer perpendiculairement à la direction (strafe gauche)
    new_x = game->player.x - game->player.dir_y * MOVE_SPEED;
    new_y = game->player.y + game->player.dir_x * MOVE_SPEED;
    
    // Vérifier collision X
    if (!is_wall(game, (int)new_x, (int)game->player.y))
        game->player.x = new_x;
    
    // Vérifier collision Y
    if (!is_wall(game, (int)game->player.x, (int)new_y))
        game->player.y = new_y;
    
    printf("Player strafed left to: (%.2f, %.2f)\n", game->player.x, game->player.y);
}

void move_right(t_game *game)
{
    double new_x, new_y;
    
    // Se déplacer perpendiculairement à la direction (strafe droite)
    new_x = game->player.x + game->player.dir_y * MOVE_SPEED;
    new_y = game->player.y - game->player.dir_x * MOVE_SPEED;
    
    // Vérifier collision X
    if (!is_wall(game, (int)new_x, (int)game->player.y))
        game->player.x = new_x;
    
    // Vérifier collision Y
    if (!is_wall(game, (int)game->player.x, (int)new_y))
        game->player.y = new_y;
    
    printf("Player strafed right to: (%.2f, %.2f)\n", game->player.x, game->player.y);
}

void rotate_left(t_game *game)
{
    double old_dir_x, old_plane_x;
    
    old_dir_x = game->player.dir_x;
    old_plane_x = game->player.plane_x;
    
    // Rotation de -ROT_SPEED radians (sens anti-horaire)
    game->player.dir_x = game->player.dir_x * cos(-ROT_SPEED) - game->player.dir_y * sin(-ROT_SPEED);
    game->player.dir_y = old_dir_x * sin(-ROT_SPEED) + game->player.dir_y * cos(-ROT_SPEED);
    
    game->player.plane_x = game->player.plane_x * cos(-ROT_SPEED) - game->player.plane_y * sin(-ROT_SPEED);
    game->player.plane_y = old_plane_x * sin(-ROT_SPEED) + game->player.plane_y * cos(-ROT_SPEED);
    
    printf("Player rotated left. New direction: (%.2f, %.2f)\n", 
           game->player.dir_x, game->player.dir_y);
}

void rotate_right(t_game *game)
{
    double old_dir_x, old_plane_x;
    
    old_dir_x = game->player.dir_x;
    old_plane_x = game->player.plane_x;
    
    // Rotation de +ROT_SPEED radians (sens horaire)
    game->player.dir_x = game->player.dir_x * cos(ROT_SPEED) - game->player.dir_y * sin(ROT_SPEED);
    game->player.dir_y = old_dir_x * sin(ROT_SPEED) + game->player.dir_y * cos(ROT_SPEED);
    
    game->player.plane_x = game->player.plane_x * cos(ROT_SPEED) - game->player.plane_y * sin(ROT_SPEED);
    game->player.plane_y = old_plane_x * sin(ROT_SPEED) + game->player.plane_y * cos(ROT_SPEED);
    
    printf("Player rotated right. New direction: (%.2f, %.2f)\n", 
           game->player.dir_x, game->player.dir_y);
}