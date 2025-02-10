#include "minishell.h"

t_error	ft_strtok_mnsh(char *s, t_token **tokis)
{
	int		i;
	t_error	tok_status;

	i = 0;
	while (*s)
	{
		tok_status = tok_check(&s, tokis, &i);
		if (tok_status == CONTINUE)
			continue;
		else if (tok_status > 0)
			return (tok_status);
	}
	(*tokis)[i].word = NULL;
	(*tokis)[i].type = 0;
	return (ERR_NOERR);
}

t_error	tok_check(char **s, t_token **tokis, int *i)
{
	t_error	status;

	if (tok_check_spaces(s))
		return (CONTINUE);
	status = tok_check_operator(s, tokis, i);
	if (status == CONTINUE)
		return (CONTINUE);
	else if (status > 0)
		return (status);
	// status = tok_check_quotes(s, tokis, i);
	// if (status == CONTINUE)
	// 	return (CONTINUE);
	// else if (status > 0)
	// 	return (status);
	status = tok_check_regular(s, tokis, i);
	if (status == CONTINUE)
		return (CONTINUE);
	else
		return (status);
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

t_error	tok_check_operator(char **s, t_token **tokis, int *i)
{
	int	len_operator;

	len_operator = is_operator(*s, TOK_OPERATORS);
	if (len_operator > 0)
	{
		(*tokis)[*i].word = ft_strndup(*s, len_operator);
		(*tokis)[*i].type = TOKEN_OPERATOR;
		if ((*tokis)[*i].word == NULL)
			return (perror_malloc("tok_check_operator"));
		(*i)++;
		(*s) += len_operator;
		return (CONTINUE);
	}
	return (ERR_NOERR);
}

t_error	tok_check_regular(char **s, t_token **tokis, int *i)
{
	char	*start;

	start = *s;
	while (**s && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS))
		(*s)++;
	(*tokis)[*i].word = ft_strndup(start, *s - start);
	(*tokis)[*i].type = TOKEN_WORD;
	if ((*tokis)[*i].word == NULL)
		return (perror_malloc("tok_check_regular"));
	(*i)++;
	return (CONTINUE);
}

t_token	*new_toki(char *word, t_token_type type)
{
	t_token	*toki;

	toki = ft_calloc(1, sizeof(t_token));
	if (!toki)
		return (NULL);
	toki->word = word;
	toki->type = type;
	toki->next = NULL;
	return (toki);
}



// t_error	tok_check_quotes(char **s, t_token **tokis, int *i)
// {
// 	char	quote;
// 	char	*start;

// 	if (**s == '\'' || **s == '"')
// 	{
// 		quote = **s;
// 		(*s)++;
// 		start = *s - 1;
// 		while (**s && **s != quote)
// 			(*s)++;
// 		if (**s == '\0')
// 		{
// 			ft_free_tokens(tokis);
// 			return (mnsh_perror(ERR_QUOTE));
// 		}
// 		(*tokis)[*i].word = ft_strndup(start, *s - start + 1);
// 		(*tokis)[*i].type = TOKEN_WORD;
// 		if ((*tokis)[*i].word == NULL)
// 			return (perror_malloc("tok_check_quotes"));
// 		(*i)++;
// 		(*s)++;
// 		return (CONTINUE);
// 	}
// 	return (ERR_NOERR);
// }


