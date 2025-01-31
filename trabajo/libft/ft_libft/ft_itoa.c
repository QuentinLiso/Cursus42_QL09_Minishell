/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:55:31 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:55:47 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_numlen(int n)
{
	size_t	len;

	len = 0;
	while (n / 10)
	{
		n /= 10;
		len++;
	}
	return (len + 1);
}

static char	*ft_negitoa(int n)
{
	char	*dest;
	size_t	len;

	len = ft_numlen(n) + 1;
	dest = malloc (sizeof(char) * (len + 1));
	if (!dest)
		return (0);
	dest[0] = '-';
	dest[len] = '\0';
	len--;
	while (len)
	{
		dest[len] = (n % 10) * -1 + '0';
		n /= 10;
		len--;
	}
	return (dest);
}

static char	*ft_positoa(int n)
{
	char	*dest;
	size_t	len;

	len = ft_numlen(n);
	dest = malloc (sizeof(char) * (len + 1));
	if (!dest)
		return (0);
	dest[len] = '\0';
	len--;
	while (len)
	{
		dest[len] = (n % 10) + '0';
		n /= 10;
		len--;
	}
	dest[len] = (n % 10) + '0';
	return (dest);
}

char	*ft_itoa(int n)
{
	if (n < 0)
		return (ft_negitoa(n));
	else
		return (ft_positoa(n));
}
