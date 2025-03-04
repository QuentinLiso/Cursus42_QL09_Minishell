/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20_tokenize_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:48:48 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:31 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tok_check_ope(t_mnsh *mnsh, char **s, t_toktype type)
{
	int	ope_len;
	int	status;

	ope_len = mnsh_special(*s, mnsh->tok_operators);
	if (ope_len > 0)
	{
		status = add_toki_mnsh(mnsh, ft_substr(*s, 0, (size_t)ope_len), type);
		if (status)
			return (status);
		(*s) += ope_len;
		return (1);
	}
	return (ope_len);
}

int	tok_check_ind(t_mnsh *mnsh, char **s, t_toktype type)
{
	int	ope_len;
	int	status;

	ope_len = mnsh_special(*s, mnsh->tok_indir);
	if (ope_len > 0)
	{
		status = add_toki_mnsh(mnsh, ft_substr(*s, 0, (size_t)ope_len), type);
		if (status)
			return (status);
		(*s) += ope_len;
		return (1);
	}
	return (ope_len);
}

int	tok_check_regular(t_mnsh *mnsh, char **s)
{
	char	*word;
	int		status;
	bool	has_quote;

	word = NULL;
	has_quote = false;
	while (**s && !ft_isspace(**s) && !mnsh_special(*s, mnsh->tok_operators)
		&& !mnsh_special(*s, mnsh->tok_indir))
	{
		status = tok_check_regular_split(mnsh, s, &word, &has_quote);
		if (status < 0)
		{
			free_str(&word);
			return (status);
		}
		else if (status > 0)
			continue ;
	}
	if ((word && !*word && has_quote) || (word && has_quote)
		|| (word && *word && !has_quote))
		return (add_toki_mnsh(mnsh, word, TOKEN_WORD));
	else
		free_str(&word);
	return (1);
}

int	tok_check_regular_split(t_mnsh *mnsh, char **s, char **word, bool *q)
{
	int	status;

	status = split_quote(s, word);
	if (status)
	{
		*q = true;
		return (status);
	}
	status = split_dquote(mnsh, s, word);
	if (status)
	{
		*q = true;
		return (status);
	}
	status = split_noquote(mnsh, s, word);
	if (status)
		return (status);
	return (0);
}

int	split_quote(char **s, char **word)
{
	char	*buffer;
	char	*start;

	if (**s != '\'')
		return (0);
	(*s)++;
	start = *s;
	while (**s && **s != '\'')
		(*s)++;
	if (!**s)
		return (perror_mnsh(-2, 1, "Open quote error"));
	buffer = ft_substr(start, 0, *s - start);
	if (!buffer)
		return (-12);
	*word = ft_strappend_mnsh(*word, buffer);
	free_str(&buffer);
	if (!*word)
		return (-12);
	(*s)++;
	return (1);
}
