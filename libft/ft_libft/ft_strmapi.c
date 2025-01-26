/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:56:11 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:56:49 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	slen;
	size_t	i;
	char	*dest;

	slen = ft_strlen(s);
	dest = malloc(sizeof(char) * (slen + 1));
	if (!dest)
		return (0);
	i = 0;
	if (s && f)
	{
		while (s[i])
		{
			dest[i] = f(i, s[i]);
			i++;
		}
	}
	dest[i] = '\0';
	return (dest);
}
