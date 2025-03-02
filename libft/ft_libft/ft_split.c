/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:54:59 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:55:24 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_numwords(char const *s, char c)
{
	size_t	count;
	size_t	elapsed;

	count = 0;
	elapsed = 0;
	while (*s)
	{
		if (*s == c && elapsed)
		{
			elapsed = 0;
			count++;
		}
		else if (*s != c)
			elapsed++;
		s++;
	}
	if (elapsed)
		count++;
	return (count);
}

static size_t	ft_strlenchar(const char *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		++len;
	return (len);
}

char	**new_str(char **dest, char *s, size_t words, char c)
{
	size_t	i;

	i = 0;
	while (i < words)
	{
		while (*s == c)
			s++;
		dest[i] = malloc (sizeof(char) * (ft_strlenchar(s, c) + 1));
		if (!dest[i])
		{
			while (i > 0)
				free(dest[i--]);
			free(dest[i]);
			free(dest);
			return (NULL);
		}
		ft_strlcpy(dest[i], s, ft_strlenchar(s, c) + 1);
		s += ft_strlenchar(s, c);
		i++;
	}
	return (dest);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**dest;

	words = ft_numwords(s, c);
	dest = ft_calloc (words + 1, sizeof(char *));
	if (!dest)
		return (NULL);
	dest = new_str(dest, (char *)s, words, c);
	return (dest);
}
