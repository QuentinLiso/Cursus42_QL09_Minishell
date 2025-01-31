/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:53 by qliso             #+#    #+#             */
/*   Updated: 2024/11/21 20:23:56 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*buf_read;
	static char	*buf_line[FD_MAX];
	char		*line;

	line = NULL;
	buf_read = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0)
	{
		free(buf_line[fd]);
		free(buf_read);
		buf_line[fd] = NULL;
		buf_read = NULL;
		return (NULL);
	}
	if (!buf_read)
		return (NULL);
	buf_line[fd] = set_buf_line(fd, buf_line[fd], buf_read);
	if (*buf_line[fd] == 0)
	{
		free (buf_line[fd]);
		return (buf_line[fd] = 0);
	}
	line = set_line(buf_line[fd], line);
	buf_line[fd] = offset_buf_line(buf_line[fd]);
	return (line);
}

char	*set_buf_line(int fd, char *buf_line, char *buf_read)
{
	ssize_t	nbytes;

	nbytes = 1;
	if (!buf_line)
		buf_line = ft_strdup_gnl("");
	while (nbytes > 0)
	{
		nbytes = read(fd, buf_read, BUFFER_SIZE);
		if (nbytes == -1)
		{
			free (buf_read);
			return (NULL);
		}
		buf_read[nbytes] = '\0';
		buf_line = ft_strjoin_gnl(buf_line, buf_read);
		if ((ft_strchr_gnl(buf_read, '\n')))
			break ;
	}
	free (buf_read);
	return (buf_line);
}

char	*set_line(char *buf_line, char *line)
{
	int	start;
	int	i;

	start = 0;
	i = 0;
	if (!buf_line)
		return (NULL);
	while (buf_line[start] != '\n' && buf_line[start])
		start++;
	if (buf_line[start] == '\n')
		start++;
	line = malloc((start + 1) * sizeof(char));
	if (!line)
		return (NULL);
	while (i < start)
	{
		line[i] = buf_line[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*offset_buf_line(char	*buf_line)
{
	char	*new_bline;
	int		start;
	int		i;

	start = 0;
	i = 0;
	if (!buf_line)
		return (NULL);
	while (buf_line[start] != '\n' && buf_line[start])
		start++;
	if (buf_line[start] == '\n')
		start++;
	new_bline = malloc((ft_strlen_gnl(buf_line) - start + 1) * sizeof(char));
	if (!new_bline)
		return (NULL);
	while (buf_line[start + i])
	{
		new_bline[i] = buf_line[start + i];
		i++;
	}
	free (buf_line);
	new_bline[i] = '\0';
	return (new_bline);
}

/*
#include <stdio.h>
int main()
{
	int	fda = open("text.txt", O_RDONLY);
	int	fdb = open("text.txt", O_RDONLY);
	int	fdc = open("text.txt", O_RDONLY);

	printf("1 A: %s", get_next_line(fda));
	printf("2 A: %s", get_next_line(fda));
	printf("3 A: %s", get_next_line(fda));
	printf("1 B: %s", get_next_line(fdb));
	printf("2 B: %s", get_next_line(fdb));
	printf("4 A: %s", get_next_line(fda));
	printf("1 C: %s", get_next_line(fdc));
	printf("2 C: %s", get_next_line(fdc));
	printf("3 C: %s", get_next_line(fdc));
	printf("5 A: %s", get_next_line(fda));
	printf("6 A: %s", get_next_line(fda));
	close(fda);
	close(fdb);
	close(fdc);
}
*/