/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   70_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:16:17 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:30:15 by qliso            ###   ########.fr       */
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

bool	mnsh_lookup(char c, char *s)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if (s[i] == c)
			return (true);
	}
	return (false);
}

int	mnsh_special(char *s, char **operators)
{
	int		i;
	int		ope_len;

	i = -1;
	while (operators[++i])
	{
		ope_len = ft_strlen(operators[i]);
		if (!ft_strncmp(s, operators[i], ope_len))
			return (ope_len);
	}
	return (0);
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
