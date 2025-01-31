/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:40:44 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:41:15 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*buf_d;
	unsigned char	*buf_s;

	if (!dest && !src)
		return (0);
	buf_d = (unsigned char *)dest;
	buf_s = (unsigned char *)src;
	while (n > 0)
	{
		*(buf_d++) = *(buf_s++);
		n--;
	}
	return (dest);
}
