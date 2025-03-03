/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   70_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:16:17 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 17:19:25 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_isspace(char c)
{
	return (c == '\f' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == ' ');
}

bool	isquote(char c)
{
	return (c == '\'' || c == '"');
}

bool	ft_isspecial(char c, const char *list_specials)
{
	char	*specials;

	specials = (char *)list_specials;
	while (*specials)
	{
		if (c == *specials)
			return (true);
		specials++;
	}
	return (false);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	ft_strarrlen(char **arr)
{
	int	len;

	if (!arr)
		return (0);
	len = 0;
	while (arr[len])
		len++;
	return (len);
}
