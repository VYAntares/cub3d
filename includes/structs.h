/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:53:45 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/03 14:33:35 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
# define TEX_WIDTH 64
# define TEX_HEIGHT 64
# define MOVE_SPEED 0.05
# define ROT_SPEED 0.03

typedef	struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef	struct s_textures
{
	char	*north_texture;
	char	*south_texture;
	char	*west_texture;
	char	*east_texture;
}	t_textures;

typedef struct s_setup
{
	t_textures	textures;
	t_color		floor;
	t_color		ceiling;
	char		**map;
	int			map_height;
	int			map_width;
}	t_setup;

#endif