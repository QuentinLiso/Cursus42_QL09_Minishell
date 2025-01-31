/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:52:56 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:53:33 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	d_len;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	else
	{
		d_len = ft_strlen(s + start);
		if (len < d_len)
			d_len = len;
		dest = malloc ((d_len + 1) * sizeof(char));
		if (!dest)
			return (NULL);
		dest[d_len] = '\0';
		while (d_len-- > 0)
			dest[d_len] = s[start + d_len];
	}
	return (dest);
}
