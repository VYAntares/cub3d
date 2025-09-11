/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:12:08 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 18:53:04 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	char_player(char c)
{
	return (c == 'W' || c == 'E' || c == 'S' || c == 'N');
}

int	check_char(char ok)
{
	return (ok == '1' || ok == '0' || ok == ' ' || char_player(ok));
}

int	wall_and_player_check(t_setup *setup, int *player_count, int i, int j)
{
	if (char_player(setup->map[i][j]))
	{
		(*player_count)++;
		setup->spawn_x = j;
		setup->spawn_y = i;
		setup->spawn_dir = setup->map[i][j];
	}
	if (i == 0 || j == 0
		|| j == setup->map_width - 1 || i == setup->map_height - 1)
		return (1);
	if (setup->map[i][j + 1] == ' ' || setup->map[i][j - 1] == ' '
		|| setup->map[i + 1][j] == ' ' || setup->map[i - 1][j] == ' ')
		return (1);
	return (0);
}

int	validate_map(t_setup *setup)
{
	int	i;
	int	j;
	int	player_count;

	player_count = 0;
	i = 0;
	while (i < setup->map_height)
	{
		j = 0;
		while (j < setup->map_width)
		{
			if (!check_char(setup->map[i][j]))
				return (printf("1\n"), 1);
			if (char_player(setup->map[i][j]) || setup->map[i][j] == '0')
				if (wall_and_player_check(setup, &player_count, i, j) == 1)
					return (1);
			j++;
		}
		i++;
	}
	if (player_count != 1)
		return (1);
	setup->map[setup->spawn_y][setup->spawn_x] = '0';
	return (0);
}
