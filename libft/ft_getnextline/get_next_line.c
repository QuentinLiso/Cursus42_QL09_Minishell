/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qzoli <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 22:27:08 by qzoli             #+#    #+#             */
/*   Updated: 2025/01/16 22:27:10 by qzoli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	int			endl;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	endl = -1;
	line = init_line_from_buffer(buffer, &endl);
	if (!line)
		return (NULL);
	ft_strlcpy_gnl(buffer, &buffer[endl + 1], BUFFER_SIZE + 1);
	line = fill_line_from_reader(line, buffer, &endl, fd);
	if (!line || line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*init_line_from_buffer(char *buffer, int *endl)
{
	size_t	len;
	char	*line;

	len = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_memcpy_gnl(line, buffer, len);
	line[len] = '\0';
	if (len > 0 && line[len - 1] == '\n')
		*endl = len - 1;
	return (line);
}

char	*fill_line_from_reader(char *line, char *buffer, int *endl, int fd)
{
	char	reader[BUFFER_SIZE + 1];
	ssize_t	read_check;
	size_t	line_size;

	while (*endl == -1)
	{
		ft_bzero_gnl(reader, (BUFFER_SIZE + 1));
		read_check = read(fd, reader, BUFFER_SIZE);
		if (read_check == -1)
		{
			free(line);
			ft_bzero_gnl(buffer, (BUFFER_SIZE + 1));
			return (NULL);
		}
		line_size = find_newline(reader);
		ft_strlcpy_gnl(buffer, &reader[line_size], (BUFFER_SIZE + 1));
		reader[line_size] = '\0';
		line = ft_strappend_gnl(line, reader, endl);
		if (read_check == 0)
		{
			ft_bzero_gnl(buffer, BUFFER_SIZE + 1);
			break ;
		}
	}
	return (line);
}

size_t	find_newline(char *reader)
{
	size_t	i;

	i = 0;
	if (!reader)
		return (-1);
	while (i < BUFFER_SIZE)
	{
		if (reader[i] == '\n' || reader[i] == '\0')
			return (i + 1);
		i++;
	}
	return (i);
}