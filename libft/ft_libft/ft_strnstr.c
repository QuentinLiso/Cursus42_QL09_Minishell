/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:50:35 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:51:10 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	l_len;

	if (!*little)
		return ((char *)big);
	i = 0;
	l_len = ft_strlen(little);
	while (big[i] && i + l_len - 1 < len)
	{
		j = 0;
		while (little[j] && little[j] == big[i + j])
		{
			if (j == l_len - 1)
				return ((char *)&big[i]);
			j++;
		}
		i++;
	}
	return (0);
}
