/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20_tokenize_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:52:30 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:33 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_noquote_env_spec(t_mnsh *mnsh, char **s, char **buffer)
{
	if (**s == '?')
		return (get_last_exit(mnsh, s, buffer));
	else if (mnsh_lookup(**s, mnsh->env_specials))
	{
		(*s)++;
		return (1);
	}
	else if (!ft_isalnum(**s) && **s != '_' && !isquote(**s))
	{
		*buffer = ft_strappend_mnsh(*buffer, "$");
		if (!*buffer)
			return (-12);
		return (1);
	}
	return (0);
}

int	split_noquote_noenv(t_mnsh *mnsh, char **s, char **buffer)
{
	char	*start;
	char	*buffer_2;

	start = *s;
	while (**s && !ft_isspace(**s) && !mnsh_special(*s, mnsh->tok_operators)
		&& !mnsh_special(*s, mnsh->tok_indir) && !mnsh_lookup(**s, "'\"$"))
		(*s)++;
	buffer_2 = ft_substr(start, 0, *s - start);
	if (!buffer_2)
		return (-12);
	*buffer = ft_strappend_mnsh(*buffer, buffer_2);
	free_str(&buffer_2);
	if (!*buffer)
		return (-12);
	return (0);
}

int	append_noquote(char **word, char **buffer)
{
	if (*buffer)
		*word = ft_strappend_mnsh(*word, *buffer);
	free_str(buffer);
	if (!*word)
		return (-12);
	return (1);
}
