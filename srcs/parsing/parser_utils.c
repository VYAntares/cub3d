/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:47:50 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/03 14:51:17 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int validate_file(char *filename)
{
    int len;
	int	fd;
	
    len = ft_strlen(filename);
    if ((len < 5) || ft_strncmp(&filename[len - 4], ".cub", 4) != 0)
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		error_exit(ERR_FILE);
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
}

char	*trim_line(char *line)
{
	int			i;
	int			j;
	static char	trimmed[4096];

	i = 0;
	j = 0;
    while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
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
