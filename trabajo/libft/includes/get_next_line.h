/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qzoli <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 22:27:33 by qzoli             #+#    #+#             */
/*   Updated: 2025/01/16 22:27:39 by qzoli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif
# if BUFFER_SIZE > 100000 || BUFFER_SIZE < 1
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
char	*init_line_from_buffer(char *buffer, int *endl);
char	*fill_line_from_reader(char *line, char *buffer, int *endl, int fd);
size_t	find_newline(char *reader);

size_t	ft_strlen_gnl(const char *s);
void	ft_bzero_gnl(void *s, size_t n);
void	*ft_memcpy_gnl(void *dst, const void *src, size_t n);
char	*ft_strappend_gnl(char *s1, char *s2, int *endl);
void	ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize);

#endif