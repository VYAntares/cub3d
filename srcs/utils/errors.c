/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:24:05 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 19:23:35 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

void	clean_textures(t_textures *textures)
{
	if (!textures)
		return ;
	if (textures->north_texture)
	{
		free(textures->north_texture);
		textures->north_texture = NULL;
	}
	if (textures->south_texture)
	{
		free(textures->south_texture);
		textures->south_texture = NULL;
	}
	if (textures->west_texture)
	{
		free(textures->west_texture);
		textures->west_texture = NULL;
	}
	if (textures->east_texture)
	{
		free(textures->east_texture);
		textures->east_texture = NULL;
	}
}

void	clean_map(t_setup *setup)
{
	int	i;

	if (!setup || !setup->map)
		return ;
	i = 0;
	while (i < setup->map_height && setup->map[i])
	{
		free(setup->map[i]);
		setup->map[i] = NULL;
		i++;
	}
	free(setup->map);
	setup->map = NULL;
	setup->map_height = -1;
	setup->map_width = -1;
}

void	reset_colors(t_setup *setup)
{
	if (!setup)
		return ;
	setup->floor.r = -1;
	setup->floor.g = -1;
	setup->floor.b = -1;
	setup->ceiling.r = -1;
	setup->ceiling.g = -1;
	setup->ceiling.b = -1;
}

void	clean_up(t_setup *setup)
{
	if (!setup)
		return ;
	clean_textures(&setup->textures);
	clean_map(setup);
	reset_colors(setup);
}

int	error_exit(char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
	return (1);
}

