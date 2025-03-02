/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:45:54 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:46:25 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	char	*last;
	char	c_char;

	ptr = (char *)s;
	last = (char *)0;
	c_char = (char)c;
	while (*ptr)
	{
		if (*ptr == c_char)
			last = ptr;
		ptr++;
	}
	if (c_char == '\0')
		return (ptr);
	return (last);
}
