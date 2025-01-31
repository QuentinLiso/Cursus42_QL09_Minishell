/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:45:18 by qliso             #+#    #+#             */
/*   Updated: 2024/11/05 16:45:46 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ptr;
	char	c_char;

	ptr = (char *) s;
	c_char = (char) c;
	while (*ptr && *ptr != c_char)
		ptr++;
	if (*ptr == c_char)
		return (ptr);
	else
		return (NULL);
}
