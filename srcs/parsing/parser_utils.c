/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:47:50 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/11 18:56:15 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	**ft_realloc_array(char **old_array, int old_count, size_t new_size)
{
	char	**new_array;
	int		i;

	new_array = malloc(new_size);
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < old_count)
	{
		new_array[i] = old_array[i];
		i++;
	}
	free(old_array);
	return (new_array);
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

int	validate_config_complete(t_setup *setup)
{
	if (!setup->textures.north_texture
		|| !setup->textures.south_texture
		|| !setup->textures.west_texture
		|| !setup->textures.east_texture)
		return (1);
	if (setup->floor.r == -1
		|| setup->floor.g == -1
		|| setup->floor.b == -1
		|| setup->ceiling.r == -1
		|| setup->ceiling.g == -1
		|| setup->ceiling.b == -1)
		return (1);
	return (0);
}

int	validate_file(char *filename)
{
	int	len;
	int	fd;

	len = ft_strlen(filename);
	if ((len < 5) || ft_strncmp(&filename[len - 4], ".cub", 4) != 0)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
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

int	substr_map_line_wo_bn(t_setup *setup, char *line, int map_count)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		setup->map[map_count] = ft_substr(line, 0, len - 1);
	else
		setup->map[map_count] = ft_strdup(line);
	if (!setup->map[map_count])
		return (1);
	return (0);
}

char	*trim_line(char *line)
{
	int			i;
	int			j;
	char		*trimmed;

	i = 0;
	j = 0;
	trimmed = malloc(ft_strlen(line) + 1);
	if (!trimmed)
		return (NULL);
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
			i++;
		else
		{
			trimmed[j] = line[i];
			j++;
			i++;
		}
	}
	trimmed[j] = '\0';
	return (trimmed);
}
