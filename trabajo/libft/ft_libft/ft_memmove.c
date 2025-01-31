/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:41:30 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:42:05 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*buf_d;
	unsigned char	*buf_s;

	if (!dest && !src)
		return (0);
	buf_d = (unsigned char *)dest;
	buf_s = (unsigned char *)src;
	if (buf_d < buf_s)
		while (n--)
			*(buf_d++) = *(buf_s++);
	else
	{
		buf_d += (n - 1);
		buf_s += (n - 1);
		while (n--)
			*(buf_d--) = *(buf_s--);
	}
	return (dest);
}
