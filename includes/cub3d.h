/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:14:37 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/03 16:23:22 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <fcntl.h>
# include "mlx.h"
# include "libft.h"
# include "structs.h"

// Key codes (MacOS)
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_ESC 53

// Key codes (Linux)
// # define KEY_W 119
// # define KEY_A 97
// # define KEY_S 115
// # define KEY_D 100
// # define KEY_LEFT 65361
// # define KEY_RIGHT 65363
// # define KEY_ESC 65307

// Events
# define ON_KEYDOWN 2
# define ON_KEYUP 3
# define ON_DESTROY 17

# define ERR_FILECONFIG 1001
# define MAP_FOUND 1002
# define ERROR_COLOR 1003
# define ERROR_TEXTURE 1004
# define ERROR_PARSE 1005


// Error messages
# define ERR_ARGS "Error\nUsage: ./cub3D <map.cub>\n"
# define ERR_FILE "Error\nCannot open file\n"
# define ERR_EXTENSION "Error\nFile must have .cub extension\n"
# define ERR_MALLOC "Error\nMemory allocation failed\n"
# define ERR_PARSE "Error\nInvalid .cub file\n"
# define ERR_TEXTURE "Error\nInvalid texture path\n"
# define ERR_COLOR "Error\nInvalid color format\n"
# define ERR_MLX "Error\nMLX initialization failed\n"

int		main(int ac, char **av);
void	error_exit(char *msg);
int		validate_file(char *filename);
void	parser(char *filename);
int		parse_config(char *line, t_setup *setup);
char	*trim_line(char *line);
void	init_setup(t_setup *setup);

#endif