/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ardemiri <Demiriardit9mail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:36:20 by ardemiri          #+#    #+#             */
/*   Updated: 2026/03/25 18:36:20 by ardemiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*gnl_join_and_free(char *stash, char *buf);
char	*gnl_find_newline(char *s);

static char	*extract_line(char **stash)
{
	char	*line;
	char	*new_stash;
	size_t	i;

	if (!*stash || **stash == '\0')
		return (NULL);
	i = 0;
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	if ((*stash)[i] == '\n')
		i++;
	line = ft_substr(*stash, 0, i);
	if (!line)
		return (NULL);
	new_stash = ft_strdup(*stash + i);
	free(*stash);
	*stash = new_stash;
	return (line);
}

static int	read_into_stash(int fd, char **stash)
{
	char	*buf;
	int		bytes;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (FAILURE);
	bytes = 1;
	while (!gnl_find_newline(*stash) && bytes > 0)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buf);
			return (FAILURE);
		}
		buf[bytes] = '\0';
		*stash = gnl_join_and_free(*stash, buf);
		if (!*stash)
		{
			free(buf);
			return (FAILURE);
		}
	}
	free(buf);
	return (SUCCESS);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!read_into_stash(fd, &stash))
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = extract_line(&stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}
