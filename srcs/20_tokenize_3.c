/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20_tokenize_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:49:40 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:32 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_dquote(t_mnsh *mnsh, char **s, char **word)
{
	char	*buffer;
	int		status;

	if (**s != '"')
		return (0);
	buffer = ft_strdup("");
	if (!buffer)
		return (perror_mnsh(-12, 1, "Err malloc in split dquote"));
	(*s)++;
	while (**s && **s != '"')
	{
		status = check_split_dquote(mnsh, s, &buffer);
		if (status < 0)
		{
			free_str(&buffer);
			return (status);
		}
		else if (status > 0)
			continue ;
	}
	return (append_dquote(s, word, &buffer));
}

int	check_split_dquote(t_mnsh *mnsh, char **s, char **buffer)
{
	int	status;

	status = split_dquote_env(mnsh, s, buffer);
	if (status)
		return (status);
	status = split_dquote_noenv(s, buffer);
	if (status)
		return (status);
	return (0);
}

int	split_dquote_env(t_mnsh *mnsh, char **s, char **buffer)
{
	char	*start;
	char	*env;
	int		status;

	if (**s != '$')
		return (0);
	(*s)++;
	start = *s;
	status = split_dquote_env_spec(mnsh, s, buffer);
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

int	split_dquote_env_spec(t_mnsh *mnsh, char **s, char **buffer)
{
	if (**s == '?')
		return (get_last_exit(mnsh, s, buffer));
	else if (mnsh_lookup(**s, mnsh->env_specials))
	{
		*buffer = ft_strappend_mnsh(*buffer, "");
		if (!*buffer)
			return (-12);
		(*s)++;
		return (1);
	}
	else if (!ft_isalnum(**s) && **s != '_')
	{
		*buffer = ft_strappend_mnsh(*buffer, "$");
		if (!*buffer)
			return (-12);
		return (1);
	}
	return (0);
}

int	split_dquote_noenv(char **s, char **buffer)
{
	char	*start;
	char	*buffer_2;

	start = *s;
	while (**s && **s != '$' && **s != '"')
		(*s)++;
	buffer_2 = ft_substr(start, 0, *s - start);
	if (!buffer_2)
		return (-12);
	*buffer = ft_strappend_mnsh(*buffer, buffer_2);
	if (!*buffer)
		return (-12);
	free_str(&buffer_2);
	return (0);
}
