#include "minishell.h"

t_error	ft_strtok_mnsh(char *s, char ***tokens)
{
	int		i;
	t_error	tok_status;

	i = 0;
	while (*s)
	{
		tok_status = tok_check(&s, tokens, &i);
		if (tok_status == CONTINUE)
			continue;
		else if (tok_status > 0)
			return (tok_status);
	}
	(*tokens)[i] = NULL;
	return (ERR_NOERR);
}

t_error	tok_check(char **s, char ***tokens, int *i)
{
	t_error	status;

	if (tok_check_spaces(s))
		return (CONTINUE);
	status = tok_check_operator(s, tokens, i);
	if (status == CONTINUE)
		return (CONTINUE);
	else if (status > 0)
		return (status);
	status = tok_check_quotes(s, tokens, i);
	if (status == CONTINUE)
		return (CONTINUE);
	else if (status > 0)
		return (status);
	status = tok_check_regular(s, tokens, i);
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

t_error	tok_check_operator(char **s, char ***tokens, int *i)
{
	int	len_operator;

	len_operator = is_operator(*s, TOK_OPERATORS);
	if (len_operator > 0)
	{
		(*tokens)[*i] = ft_strndup(*s, len_operator);
		if ((*tokens)[*i] == NULL)
			return (perror_malloc("tok_check_operator"));
		(*i)++;
		(*s) += len_operator;
		return (CONTINUE);
	}
	return (ERR_NOERR);
}

t_error	tok_check_quotes(char **s, char ***tokens, int *i)
{
	char	quote;
	char	*start;

	if (**s == '\'' || **s == '"')
	{
		quote = **s;
		(*s)++;
		start = *s - 1;
		while (**s && **s != quote)
			(*s)++;
		if (**s == '\0')
		{
			ft_free_strarray(tokens);
			return (mnsh_perror(ERR_QUOTE));
		}
		(*tokens)[*i] = ft_strndup(start, *s - start + 1);
		if ((*tokens)[*i] == NULL)
			return (perror_malloc("tok_check_quotes"));
		(*i)++;
		(*s)++;
		return (CONTINUE);
	}
	return (ERR_NOERR);
}

t_error	tok_check_regular(char **s, char ***tokens, int *i)
{
	char	*start;

	start = *s;
	while (**s && !ft_isspace(**s) && !ft_isspecial(**s, TOK_SPECIALS))
		(*s)++;
	(*tokens)[*i] = ft_strndup(start, *s - start);
	if ((*tokens)[*i] == NULL)
		return (perror_malloc("tok_check_regular"));
	(*i)++;
	return (CONTINUE);
}
