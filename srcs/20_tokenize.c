/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:46:53 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:34 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_toki(char *word, t_toktype type)
{
	t_token	*toki;

	toki = ft_calloc(1, sizeof(t_token));
	if (!toki)
		return (NULL);
	toki->word = word;
	toki->type = type;
	toki->next = NULL;
	toki->prev = NULL;
	return (toki);
}

int	add_toki_mnsh(t_mnsh *mnsh, char *word, t_toktype t)
{
	if (mnsh->tokis == NULL)
	{
		mnsh->tokis = new_toki(word, t);
		if (!mnsh->tokis)
			return (-12);
		mnsh->last_tokis = mnsh->tokis;
		return (0);
	}
	else
	{
		mnsh->last_tokis->next = new_toki(word, t);
		if (!mnsh->last_tokis->next)
			return (-12);
		mnsh->last_tokis->next->prev = mnsh->last_tokis;
		mnsh->last_tokis = mnsh->last_tokis->next;
		return (0);
	}
}

int	strtok_mnsh(t_mnsh *mnsh, char *s)
{
	int		status;

	status = 0;
	while (*s)
	{
		status = tok_check(mnsh, &s);
		if (status < 0)
			break ;
		else if (status > 0)
			continue ;
	}
	if (status < 0)
	{
		mnsh->last_exit_status = -status;
		return (status);
	}
	return (0);
}

int	tok_check(t_mnsh *mnsh, char **s)
{
	int	status;

	status = tok_check_spaces(s);
	if (status)
		return (status);
	status = tok_check_ope(mnsh, s, TOKEN_OPERATOR);
	if (status)
		return (status);
	status = tok_check_ind(mnsh, s, TOKEN_INDIR);
	if (status)
		return (status);
	status = tok_check_regular(mnsh, s);
	if (status)
		return (status);
	return (0);
}

int	tok_check_spaces(char **s)
{
	if (ft_isspace(**s))
	{
		(*s)++;
		return (1);
	}
	return (0);
}
