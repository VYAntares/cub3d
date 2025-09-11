#include "../../includes/cub3d.h"

void init_player(t_player *player, t_setup *setup)
{
    // Position du joueur au centre de sa case spawn
    player->x = (double)setup->spawn_x + 0.5;
    player->y = (double)setup->spawn_y + 0.5;
    
    // Direction selon l'orientation spawn
    if (setup->spawn_dir == 'N')
    {
        player->dir_x = 0.0;
        player->dir_y = -1.0;
        player->plane_x = 0.66;  // FOV ~66 degrés
        player->plane_y = 0.0;
    }
    else if (setup->spawn_dir == 'S')
    {
        player->dir_x = 0.0;
        player->dir_y = 1.0;
        player->plane_x = -0.66;
        player->plane_y = 0.0;
    }
    else if (setup->spawn_dir == 'E')
    {
        player->dir_x = 1.0;
        player->dir_y = 0.0;
        player->plane_x = 0.0;
        player->plane_y = 0.66;
    }
    else if (setup->spawn_dir == 'W')
    {
        player->dir_x = -1.0;
        player->dir_y = 0.0;
        player->plane_x = 0.0;
        player->plane_y = -0.66;
    }
}

int init_game(t_game *game, t_setup *setup)
{
    // Lier setup au game
    game->setup = setup;
    
    // Initialiser MLX
    if (init_mlx(game) != 0)
        return (1);
    
    // Charger les textures
    if (load_all_textures(game) != 0)
        return (1);
    
    // Initialiser le joueur
    init_player(&game->player, setup);
    
    return (0);
}