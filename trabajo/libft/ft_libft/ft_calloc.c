/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:51:50 by qliso             #+#    #+#             */
/*   Updated: 2024/11/30 10:45:05 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	size_t			max_size_t;
	size_t			len;
	size_t			i;

	max_size_t = (size_t) -1;
	if (size != 0 && nmemb > (max_size_t / size))
		return (NULL);
	len = nmemb * size;
	i = 0;
	ptr = malloc(len * sizeof(char));
	if (!ptr)
		return (NULL);
	while (i < len)
		ptr[i++] = '\0';
	return (ptr);
}
