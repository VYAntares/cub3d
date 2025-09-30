/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:52:03 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/30 17:26:43 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_all_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	free_rgb(char **rgb)
{
	int	i;

	i = 0;
	while (rgb[i])
		free(rgb[i++]);
	free(rgb);
}

void	init_setup(t_setup *setup)
{
	if (!setup)
		return ;
	setup->textures.north_texture = NULL;
	setup->textures.south_texture = NULL;
	setup->textures.west_texture = NULL;
	setup->textures.east_texture = NULL;
	setup->floor.r = -1;
	setup->floor.g = -1;
	setup->floor.b = -1;
	setup->ceiling.r = -1;
	setup->ceiling.g = -1;
	setup->ceiling.b = -1;
	setup->map = NULL;
	setup->map_height = -1;
	setup->map_width = -1;
	setup->spawn_x = -1;
	setup->spawn_y = -1;
}

void	find_max_width(t_setup *setup)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (i < setup->map_height)
	{
		len = ft_strlen(setup->map[i]);
		if (len > setup->map_width)
			setup->map_width = len;
		i++;
	}
}
