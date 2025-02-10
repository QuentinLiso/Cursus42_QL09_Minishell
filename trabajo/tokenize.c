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
		if (tok_check_regular(&s, &tokis, &iterator) == CONTINUE)
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

t_error tok_check_regular(char **s, t_token **tokis, t_token **iter)
{
	char	quote;
	char	*start;

	quote = 0;
	start = *s;
	while (**s && is_quoted(**s, quote))
	{
		update_quote(**s, &quote);
		(*s)++;
	}
	add_to_tok(ft_strndup(start, *s - start), tokis, iter, TOKEN_WORD);
	if (*iter == NULL)
			return (perror_malloc("tok_check_regular"));
	return (CONTINUE);
}

void	update_quote(char c, char *quote)
{
	if (*quote == 0 && (c == '\'' || c == '"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

bool	is_quoted(char c, char quote)
{
	if (quote)
		return (true);
	else if (!quote && (ft_isspace(c) || ft_isspecial(c, TOK_SPECIALS)))
		return (false);
	else
		return (true);
}

// t_error	tok_check_regular(char **s, t_token **tokis, t_token **iter)
// {
// 	char	*start;
// 	bool	quoted;

// 	start = *s;
// 	while (**s && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS))
// 	{
// 		if (**s )
// 		(*s)++;
// 	}
		
// 	add_to_tok(ft_strndup(start, *s - start), tokis, iter, TOKEN_WORD);
// 	if (*iter == NULL)
// 			return (perror_malloc("tok_check_regular"));
// 	return (CONTINUE);
// }

// t_error	tok_check_quotes(char **s, t_token **tokis, t_token **iter)
// {
// 	char	quote;
// 	char	*start;

// 	if (**s != '\'' && **s != '"')
// 		return (ERR_NOERR);
// 	quote = **s;
// 	start = *s;
// 	(*s)++;
// 	while (**s && **s != quote)
// 		(*s)++;
// 	if (**s == '\0')
// 	{
// 		ft_free_all_tok(tokis);
// 		return (mnsh_perror(ERR_QUOTE));
// 	}
// 	add_to_tok(ft_strndup(start, *s - start + 1), tokis, iter, TOKEN_WORD);
// 	if (*iter == NULL)
// 		return (perror_malloc("tok_check_quotes"));
// 	(*s)++;
// 	return (CONTINUE);
// }

// int	quote_status(char c, char *quote)
// {
// 	if (*quote == '\'' && c == || *quote)

// 	else if (*quote == '"')
// }