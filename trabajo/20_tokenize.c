#include "minishell.h"

// CONTINUE = 1
// GO NEXT CHECK = 0
// ERR = -1

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

int		add_toki_mnsh(t_mnsh *mnsh, char *word, t_toktype t)
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

// ==========================================

int		strtok_mnsh(t_mnsh *mnsh, char *s)
{
	int		status;

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
		ft_free_all_tok(&mnsh->tokis);
		mnsh->last_exit_status = -status;
		return (status);
	}
	return (0);
}

// ===========================================

int		tok_check(t_mnsh *mnsh, char **s)
{
	int	status;

	status = tok_check_spaces(s);
	if (status)
		return (status);
	status = tok_check_ope_ind(mnsh, s, TOK_OPERATORS, TOKEN_OPERATOR);
	if (status)
		return (status);
	status = tok_check_ope_ind(mnsh, s, TOK_INDIR, TOKEN_INDIR);
	if (status)
		return (status);
	status = tok_check_regular(mnsh, s);
	if (status)
		return (status);
	return (0);
}

// ============================================

int		tok_check_spaces(char **s)
{
	if (ft_isspace(**s))
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int		tok_check_ope_ind(t_mnsh *mnsh, char **s, char *spec, t_toktype type)
{
	int	ope_len;
	int	status;

	ope_len = is_operator(*s, spec);
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

int		is_operator(const char *s, const char *list_operators)
{
	char	**operators;
	int		i;
	size_t	len;

	operators = ft_split(list_operators, ' ');
	i = -1;
	if (!operators)
		return (-12);
	while (operators[++i])
	{
		len = ft_strlen(operators[i]);
		if (ft_strncmp(s, operators[i], len) == 0)
		{
			ft_free_strarray(&operators);
			return (len);
		}
	}
	ft_free_strarray(&operators);	
	return (0);
}


// ============================================

int		tok_check_regular(t_mnsh *mnsh, char **s)
{
	char	*word;
	int		status;
	bool	has_quote;

	word = NULL;
	has_quote = false;
	while (**s && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS))
	{
		status = tok_check_regular_split(mnsh, s, &word, &has_quote);
		if (status < 0)
			return (status);
		else if (status > 0)
			continue ;
	}
	if ((word && !*word && has_quote) || (word && has_quote) ||
		(word && *word && !has_quote))
		return (add_toki_mnsh(mnsh, word, TOKEN_WORD));
	else
		ft_free_str(&word);
	return (1);
}

int		tok_check_regular_split(t_mnsh *mnsh, char **s, char **word, bool *q)
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

// ==============================================

int		split_quote(char **s, char **word)
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
	ft_free_str(&buffer);
	if (!*word)
		return (-12);
	(*s)++;
	return (1);
}

// ==============================================

int		split_dquote(t_mnsh *mnsh, char **s, char **word)
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
			return (status);
		else if (status > 0)
			continue ;
	}
	if (!**s)
		return (perror_mnsh(-2, 1, "Open dquote error"));
	append_dquote(word, &buffer);
	if (!word)
		return (-12);
	(*s)++;
	return (1);
}

int		check_split_dquote(t_mnsh *mnsh, char **s, char **buffer)
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

int		split_dquote_env(t_mnsh *mnsh, char **s, char **buffer)
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
	ft_free_str(&env);
	if (!*buffer)
		return (-12);
	return (1);
}

int		split_dquote_env_spec(t_mnsh *mnsh, char **s, char **buffer)
{
	if (**s == '?')
		return (get_last_exit(mnsh, s, buffer));
	else if (ft_isspecial(**s, ENV_SPECIALS))
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
		(*s)++;
		return (1);
	}
	return (0);
}

int		split_dquote_noenv(char **s, char **buffer)
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
	ft_free_str(&buffer_2);
	return (0);
}

int		get_last_exit(t_mnsh *mnsh, char **s, char **buffer)
{
	char	*last_exit;

	last_exit = ft_itoa(mnsh->last_exit_status);
	if (!last_exit)
		return (-12);
	*buffer = ft_strappend_mnsh(*buffer, last_exit);
	ft_free_str(&last_exit);
	if (!*buffer)
		return (-12);
	(*s)++;
	return (1);
}

void	append_dquote(char **word, char **buffer)
{
	if (!*buffer)
		*word = ft_strappend_mnsh(*word, ft_strdup(""));
	else
		*word = ft_strappend_mnsh(*word, *buffer);
	ft_free_str(buffer);
}

// ===============================================

int		split_noquote(t_mnsh *mnsh, char **s, char **word)
{
	char	*buffer;
	int		status;

	buffer = NULL;
	while (**s && **s != '"' && **s != '\'' && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS))
	{
		status = check_split_noquote(mnsh, s, &buffer);
		if (status < 0)
			return (status);
		else if (status > 0)
			continue ;
	}
	if (buffer)
		*word = ft_strappend_mnsh(*word, buffer);
	ft_free_str(&buffer);
	if (!*word)
		return (-12);
	return (1);
}

int		check_split_noquote(t_mnsh *mnsh, char **s, char **buffer)
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

int		split_noquote_env(t_mnsh *mnsh, char **s, char **buffer)
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
	ft_free_str(&env);
	if (!*buffer)
		return (-12);
	return (1);
}

int		split_noquote_env_spec(t_mnsh *mnsh, char **s, char **buffer)
{
	if (**s == '?')
		return (get_last_exit(mnsh, s, buffer));
	else if (ft_isspecial(**s, ENV_SPECIALS))
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

int		split_noquote_noenv(char **s, char **buffer)
{
	char	*start;
	char	*buffer_2;

	start = *s;
	while (**s && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS) && !ft_isspecial(**s, "'\"$"))
		(*s)++;
	buffer_2 = ft_substr(start, 0, *s - start);
	if (!buffer_2)
		return (-12);
	*buffer = ft_strappend_mnsh(*buffer, buffer_2);
	ft_free_str(&buffer_2);
	if (!*buffer)
		return (-12);
	return (0);
}

// ================================================
