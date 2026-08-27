/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:36:02 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:36:02 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	has_cub_extension(const char *filename)
{
	int	len;

	if (!filename)
		return (0);
	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	return (1);
}

static int	check_args(int argc, char **argv)
{
	if (argc != 2)
		return (error_msg("Usage: ./cub3D <map.cub>"));
	if (!argv[1] || argv[1][0] == '\0')
		return (error_msg("Invalid map argument"));
	if (!has_cub_extension(argv[1]))
		return (error_msg("Map file must have .cub extension"));
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_game	game;

	init_game(&game);
	if (!check_args(argc, argv))
		return (FAILURE);
	if (!parse_file(argv[1], &game.scene))
		return (fail_game(&game, NULL));
	if (!validate_scene(&game.scene))
		return (fail_game(&game, NULL));
	if (!init_player_from_spawn(&game.scene))
		return (fail_game(&game, NULL));
	if (!init_mlx(&game))
		return (fail_game(&game, NULL));
	if (!load_textures(&game))
		return (fail_game(&game, NULL));
	if (!init_hooks(&game))
		return (fail_game(&game, NULL));
	mlx_loop_hook(game.mlx.mlx, game_loop, &game);
	mlx_loop(game.mlx.mlx);
	cleanup_game(&game);
	return (SUCCESS);
}
