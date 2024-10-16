/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felperei <felperei@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 09:49:51 by felperei          #+#    #+#             */
/*   Updated: 2023/11/21 09:52:14 by felperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*ft_next(char *buffer)
{
	size_t	i;
	size_t	j;
	char	*rest;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!(buffer[i]))
	{
		free(buffer);
		return (NULL);
	}
	if (buffer[i] == '\n')
		i++;
	rest = malloc(ft_strlen(buffer) - i + 1 * sizeof(char));
	if (!(rest))
		return (NULL);
	j = 0;
	while (buffer[i])
	{
		rest[j++] = buffer[i++];
	}
	rest[j++] = '\0';
	free(buffer);
	return (rest);
}

static	char	*ft_line(char *buffer)
{
	char	*line;
	size_t	i;

	i = 0;
	if (!*buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = malloc(i + 2 * sizeof(char));
	if (!(line))
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*read_file(int fd, char *buffer)
{
	char	*content;
	int		bytes_read;

	content = malloc(BUFFER_SIZE + 1);
	if (!(content))
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(buffer, '\n'))
	{
		bytes_read = read(fd, content, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(content);
			return (NULL);
		}
		content[bytes_read] = '\0';
		buffer = ft_strjoin(buffer, content);
	}
	free(content);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*content;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	content = read_file(fd, content);
	if (!(content))
		return (NULL);
	line = ft_line(content);
	content = ft_next(content);
	return (line);
}
