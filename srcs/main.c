/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:29:05 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 19:30:53 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int		main(int ac, char **av)
{
	t_game	game;
	t_setup	setup;
	
	if (ac != 2)
		error_exit(ERR_ARGS);
	if (parser(&setup, av[1]) != 0)
	{
		clean_up(&setup);
		error_exit(ERR_PARSE);
	}
	game.setup = &setup;
	if (init_mlx(&game) != 0)
    {
        clean_up(&setup);
        error_exit("MLX initialization failed\n");
    }
	setup_events(&game);
    printf("MLX initialized successfully!\n");
    
    // Pour l'instant, juste une pause pour tester
    mlx_loop(game.mlx);
    
    cleanup_mlx(&game);
    clean_up(&setup);
	return (0);
}

