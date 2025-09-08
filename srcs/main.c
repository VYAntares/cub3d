/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:29:05 by eahmeti           #+#    #+#             */
/*   Updated: 2025/09/08 23:47:14 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int		main(int ac, char **av)
{
	t_setup	setup;
	
	if (ac != 2)
		error_exit(ERR_ARGS);

	if (parser(&setup, av[1]) != 0)
	{
		printf("oui2\n");
		clean_up(&setup);
		error_exit(ERR_PARSE);
	}
	clean_up(&setup);
	printf("Parsing successful!\n");
	return (0);
}