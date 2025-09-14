/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:29:05 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/14 21:34:50 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	clean_up_gnl(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
}

int	game_loop(t_game *game)
{
	render_frame(game);
	return (0);
}

int init_game(t_game *game, t_setup *setup)
{
	game->setup = setup;
	if (init_mlx(game) != 0)
		return (1);
	if (load_all_textures(game) != 0)
		return (1);
	init_player(&game->player, setup);
	return (0);
}

int	main(int ac, char **av)
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
	if (init_game(&game, &setup) != 0)
	{
		clean_up(&setup);
		error_exit("Game initialization failed\n");
	}
	setup_events(&game);
	render_frame(&game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	clean_up(&setup);
	return (0);
}
