/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:36:43 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:36:43 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	error_msg(const char *msg)
{
	if (!msg)
		return (FAILURE);
	write(2, "Error\n", 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (FAILURE);
}

int	fail_game(t_game *game, const char *msg)
{
	if (msg)
		error_msg(msg);
	if (game)
		cleanup_game(game);
	return (FAILURE);
}
