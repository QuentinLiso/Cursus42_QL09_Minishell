/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:43:00 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:43:34 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	s_len;

	s_len = ft_strlen (src);
	if (s_len + 1 < dsize)
		ft_memcpy(dst, src, s_len + 1);
	else if (dsize != 0)
	{
		ft_memcpy(dst, src, dsize - 1);
		dst[dsize - 1] = '\0';
	}
	return (s_len);
}
