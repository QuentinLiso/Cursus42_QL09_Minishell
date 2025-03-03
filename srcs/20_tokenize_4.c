/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20_tokenize_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:51:29 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 17:47:11 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_last_exit(t_mnsh *mnsh, char **s, char **buffer)
{
	char	*last_exit;

	last_exit = ft_itoa(mnsh->last_exit_status);
	if (!last_exit)
		return (-12);
	*buffer = ft_strappend_mnsh(*buffer, last_exit);
	free_str(&last_exit);
	if (!*buffer)
		return (-12);
	(*s)++;
	return (1);
}

int	append_dquote(char **s, char **word, char **buffer)
{
	if (!**s)
	{
		free_str(buffer);
		return (perror_mnsh(-2, 1, "Open dquote error"));
	}
	if (!*buffer)
		*word = ft_strappend_mnsh(*word, ft_strdup(""));
	else
		*word = ft_strappend_mnsh(*word, *buffer);
	free_str(buffer);
	if (!word)
		return (perror_mnsh(-12, 1, "Err malloc in append dquote"));
	(*s)++;
	return (1);
}

int	split_noquote(t_mnsh *mnsh, char **s, char **word)
{
	char	*buffer;
	int		status;

	buffer = ft_strdup("");
	if (!buffer)
		return (perror_mnsh(-12, 1, "Err malloc in split noquote"));
	while (**s && **s != '"' && **s != '\'' && !ft_isspace(**s)
		&& !ft_isspecial(**s, TOK_SPECIALS))
	{
		status = check_split_noquote(mnsh, s, &buffer);
		if (status < 0)
		{
			free_str(&buffer);
			return (status);
		}
		else if (status > 0)
			continue ;
	}
	return (append_noquote(word, &buffer));
}

int	check_split_noquote(t_mnsh *mnsh, char **s, char **buffer)
{
	int	status;

	status = split_noquote_env(mnsh, s, buffer);
	if (status)
		return (status);
	status = split_noquote_noenv(s, buffer);
	if (status)
		return (status);
	return (0);
}

int	split_noquote_env(t_mnsh *mnsh, char **s, char **buffer)
{
	char	*start;
	char	*env;
	int		status;

	if (**s != '$')
		return (0);
	(*s)++;
	start = (*s);
	status = split_noquote_env_spec(mnsh, s, buffer);
	if (status)
		return (status);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env = ft_substr(start, 0, *s - start);
	if (!env)
		return (-12);
	*buffer = ft_strappend_mnsh(*buffer, get_env_var(mnsh->env_mnsh_lst, env));
	free_str(&env);
	if (!*buffer)
		return (-12);
	return (1);
}
