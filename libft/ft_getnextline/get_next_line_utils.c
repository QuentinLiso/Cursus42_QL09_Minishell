/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qzoli <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 22:28:41 by qzoli             #+#    #+#             */
/*   Updated: 2025/01/16 22:28:42 by qzoli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

void	ft_bzero_gnl(void *s, size_t n)
{
	while (n--)
		*(char *)s++ = 0;
}

void	*ft_memcpy_gnl(void *dst, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	while (n--)
		*d++ = *s++;
	return (dst);
}

char	*ft_strappend_gnl(char *s1, char *s2, int *endl)
{
	char	*d;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen_gnl(s1);
	len2 = ft_strlen_gnl(s2);
	d = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!d)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy_gnl(d, s1, len1);
	free(s1);
	ft_memcpy_gnl(d + len1, s2, len2 + 1);
	if (len1 + len2 > 0 && *(d + len1 + len2 - 1) == '\n')
		*endl = 0;
	return (d);
}

void	ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize > 0)
	{
		while (src[i] && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
}