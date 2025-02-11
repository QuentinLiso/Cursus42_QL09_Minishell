#include "minishell.h"

t_token	*ft_strtok_mnsh(char *s, t_mnsh *mnsh)
{
	t_token	*tokis;
	t_token	*iterator;

	tokis = NULL;
	iterator = NULL;
	while (*s)
	{
		if (tok_check_spaces(&s))
			continue;
		if (tok_check_operator(&s, &tokis, &iterator) == CONTINUE)
			continue;
		if (tok_check_indir(&s, &tokis, &iterator) == CONTINUE)
			continue;
		if (tok_check_regular(&s, &tokis, &iterator, mnsh) == CONTINUE)
			continue;	
	}
	mnsh->last_tokis = iterator;
	return (tokis);
}

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

t_error	add_to_tok(char *str, t_token **tok, t_token **iter, t_toktype t)
{
	if (*tok == NULL)
	{
		*tok = new_toki(str, t);
		if (!*tok)
			return (perror_malloc("add_to_tok"));
		*iter = *tok;
		(*iter)->next = NULL;
	}
	else
	{
		(*iter)->next = new_toki(str, t);
		if (!(*iter)->next)
			return (perror_malloc("add_to_tok"));
		(*iter)->next->prev = (*iter);
		(*iter) = (*iter)->next;
		(*iter)->next = NULL;
	}
	return (ERR_NOERR);
}

bool	tok_check_spaces(char **s)
{
	if (ft_isspace(**s))
	{
		(*s)++;
		return (true);
	}
	return (false);
}

t_error	tok_check_operator(char **s, t_token **tokis, t_token **iter)
{
	int	len_operator;

	len_operator = is_operator(*s, TOK_OPERATORS);
	if (len_operator > 0)
	{
		add_to_tok(ft_strndup(*s, len_operator), tokis, iter, TOKEN_OPERATOR);
		if (*iter == NULL)
			return (perror_malloc("tok_check_operator"));
		(*s) += len_operator;
		return (CONTINUE);
	}
	return (ERR_NOERR);
}

t_error	tok_check_indir(char **s, t_token **tokis, t_token **iter)
{
	int	len_operator;

	len_operator = is_operator(*s, TOK_INDIR);
	if (len_operator > 0)
	{
		add_to_tok(ft_strndup(*s, len_operator), tokis, iter, TOKEN_INDIR);
		if (*iter == NULL)
			return (perror_malloc("tok_check_operator"));
		(*s) += len_operator;
		return (CONTINUE);
	}
	return (ERR_NOERR);
}

t_error tok_check_regular(char **s, t_token **t, t_token **i, t_mnsh *mnsh)
{
	char	*buffer;

	buffer = NULL;
	while (**s && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS))
	{
		if (split_quote(s, &buffer) > 0)
			continue ;
		if (split_dquote(s, &buffer, mnsh) > 0)
			continue ;
		split_noquote(s, &buffer, mnsh);
	}
	if (buffer)
		add_to_tok(buffer, t, i, TOKEN_WORD);
	if (*i == NULL)
			return (perror_malloc("tok_check_regular"));
	return (CONTINUE);
}

int		split_quote(char **s, char **buffer)
{
	char	*dup_buf;
	char	*start;

	if (**s != '\'')
		return (0);
	// printf("split quote called\n");
	(*s)++;
	start = *s;
	while (**s && **s != '\'')
		(*s)++;
	if (!**s)
	{
		printf("Single quote error\n");
		return (-1);
	}
	dup_buf = ft_strndup(start, *s - start);
	if (!dup_buf)
		return (-1);
	*buffer = ft_strappend_mnsh(*buffer, dup_buf);
	ft_free_str(&dup_buf);
	if (!*buffer)
		return (-1);
	(*s)++;
	return (1);
}

int		split_dquote(char **s, char **buffer, t_mnsh *mnsh)
{
	char	*dup_buf;

	if (**s != '"')
		return (0);
	// printf("split dquote called\n");
	dup_buf = NULL;
	(*s)++;
	while (**s && **s != '"')
	{
		if (split_dquote_env(s, &dup_buf, mnsh) > 0)
			continue ;
		split_dquote_noenv(s, &dup_buf);
	}
	if (!**s)
	{
		printf("dquote error\n");
		return (-1);
	}
	if (!dup_buf)
		*buffer = ft_strappend_mnsh(*buffer, ft_strdup(""));
	else
		*buffer = ft_strappend_mnsh(*buffer, dup_buf);
	ft_free_str(&dup_buf);
	(*s)++;
	return (1);
}

int		split_dquote_env(char **s, char **dup_buf, t_mnsh *mnsh)
{
	char	*start;
	char	*env_buf;
	char	*env;

	if (**s != '$')
		return (0);
	(*s)++;
	start = (*s);
	if (split_dquote_env_spec(s, dup_buf, mnsh) > 0)
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env_buf = ft_strndup(start, *s - start);
	if (!env_buf)
		return (-1);
	env = ft_get_env_var(mnsh->env_mnsh, env_buf);
	*dup_buf = ft_strappend_mnsh(*dup_buf, env);
	ft_free_str(&env_buf);
	ft_free_str(&env);
	return (1);
}

int		split_dquote_env_spec(char **s, char **dup_buf, t_mnsh *mnsh)
{
	char	*dup2_buf;

	if (**s == '?')
	{
		dup2_buf = ft_itoa(mnsh->last_exit_status);
		*dup_buf = ft_strappend_mnsh(*dup_buf, dup2_buf);
		ft_free_str(&dup2_buf);
		(*s)++;
		return (1);
	}
	else if (ft_isspecial(**s, ENV_SPECIALS))
	{
		*dup_buf = ft_strappend_mnsh(*dup_buf, "");
		(*s)++;
		return (1);
	}
	else if (!ft_isalnum(**s) && **s != '_')
	{
		*dup_buf = ft_strappend_mnsh(*dup_buf, "$");
		return (1);
	}
	return (0);
}

int		split_dquote_noenv(char **s, char **dup_buf)
{
	char	*start;
	char	*dup2_buf;

	start = *s;
	while (**s && **s != '$' && **s != '"')
		(*s)++;
	dup2_buf = ft_strndup(start, *s - start);
	*dup_buf = ft_strappend_mnsh(*dup_buf, dup2_buf);
	ft_free_str(&dup2_buf);
	return (1);
}

int		split_noquote(char **s, char **buffer, t_mnsh *mnsh)
{
	char	*dup_buf;

	// printf("split noquote called\n");
	dup_buf = NULL;
	while (**s && **s != '"' && **s != '\'' && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS))
	{
		if (split_noquote_env(s, &dup_buf, mnsh) > 0)
			continue ;
		split_noquote_noenv(s, &dup_buf);
	}
	if (dup_buf)
		*buffer = ft_strappend_mnsh(*buffer, dup_buf);
	ft_free_str(&dup_buf);
	return (1);
}

int		split_noquote_env(char **s, char **dup_buf, t_mnsh *mnsh)
{
	char	*start;
	char	*env_buf;
	char	*env;

	if (**s != '$')
		return (0);
	(*s)++;
	start = (*s);
	if (split_noquote_env_spec(s, dup_buf, mnsh) > 0)
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env_buf = ft_strndup(start, *s - start);
	if (!env_buf)
		return (-1);
	env = ft_get_env_var(mnsh->env_mnsh, env_buf);
	*dup_buf = ft_strappend_mnsh(*dup_buf, env);
	ft_free_str(&env_buf);
	ft_free_str(&env);
	return (1);
}

int		split_noquote_env_spec(char **s, char **dup_buf, t_mnsh *mnsh)
{
	char	*dup2_buf;

	if (**s == '?')
	{
		dup2_buf = ft_itoa(mnsh->last_exit_status);
		*dup_buf = ft_strappend_mnsh(*dup_buf, dup2_buf);
		ft_free_str(&dup2_buf);
		(*s)++;
		return (1);
	}
	else if (ft_isspecial(**s, ENV_SPECIALS))
	{
		(*s)++;
		return (1);
	}
	else if (!ft_isalnum(**s) && **s != '_' && !isquote(**s))
	{
		*dup_buf = ft_strappend_mnsh(*dup_buf, "$");
		return (1);
	}
	return (0);
}

int		split_noquote_noenv(char **s, char **dup_buf)
{
	char	*start;
	char	*dup2_buf;

	start = *s;
	while (**s && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS) && !ft_isspecial(**s, "'\"$"))
		(*s)++;
	dup2_buf = ft_strndup(start, *s - start);
	*dup_buf = ft_strappend_mnsh(*dup_buf, dup2_buf);
	ft_free_str(&dup2_buf);
	return (1);
}

void	ft_free_all_tok(t_token **tok)
{
	t_token	*iterator;

	if (!tok)
		return ;
	while (*tok != NULL)
	{
		iterator = (*tok)->next;
		if ((*tok)->word)
			free((*tok)->word);
		free((*tok));
		*tok = iterator;
	}
}
