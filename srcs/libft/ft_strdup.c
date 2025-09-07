/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eahmeti <eahmeti@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:26:38 by lumattei          #+#    #+#             */
/*   Updated: 2025/09/07 21:35:39 by eahmeti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s;
	size_t	i;

	i = ft_strlen(s1);
	s = malloc(i + 1);
	if (!s)
		return (0);
	ft_memcpy(s, s1, i);
	s[i] = '\0';
	return (s);
}
