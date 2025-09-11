/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:29:05 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 19:44:52 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int game_loop(t_game *game)
{
    render_frame(game);
    return (0);
}

int main(int ac, char **av)
{
    t_game  game;
    t_setup setup;
    
    if (ac != 2)
        error_exit(ERR_ARGS);
    
    if (parser(&setup, av[1]) != 0)
    {
        clean_up(&setup);
        error_exit(ERR_PARSE);
    }
    
    if (init_game(&game, &setup) != 0)
    {
        clean_up(&setup);
        error_exit("Game initialization failed\n");
    }
    
    setup_events(&game);
    
    // Rendu initial
    render_frame(&game);
    
    // Hook pour le rendu continu
    mlx_loop_hook(game.mlx, game_loop, &game);
    
    printf("cub3D started successfully! Use WASD to move, arrows to look around, ESC to quit.\n");
    
    mlx_loop(game.mlx);
    
    cleanup_mlx(&game);
    clean_up(&setup);
    return (0);
}

