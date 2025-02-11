#include "minishell.h"

t_error tok_check_regular(char **s);
t_token	*strtok_subtok(char **s);
bool	is_quoted(char c, char quote);
int	tok_check_singlequote(char **s, t_token **t, t_token **i, char *quote);
int	tok_check_doublequote(char **s, t_token **t, t_token **i, char *quote);
int	tok_check_dq_env(char **s, t_token **t, t_token **i);
int	special_expand_dq(char **s, t_token **t, t_token **i);
int	tok_check_dq_noenv(char **s, t_token **t, t_token **i);

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
	if (!str)
		return (ERR_NOERR);
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

t_error tok_check_regular(char **s) //, t_token **tokis, t_token **iter, t_mnsh *mnsh)
{
	char	*value;

	value = split_regular(s);
	return (ERR_NOERR);
}

char *split_regular(char **s)
{
	char	*split;
	char	quote;

	quote = 0;
	while (**s && is_quoted(**s, quote))
	{
		if (tok_check_singlequote(s, &split, &quote) > 0)
			continue;
		if (tok_check_doublequote(s, &subtokis, &iterator, &quote) > 0)
			continue;
		if (**s)
			(*s)++;
		// tok_check_noquote(&s, &subtokis, &iterator, mnsh);
	}
	return (subtokis);
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

int	tok_check_singlequote(char **s, t_token **t, t_token **i, char *quote)
{
	char	*start;

	if (**s != '\'')
		return (0);
	printf("tok check single quote called, S : %s\n", *s);
	*quote = '\'';
	(*s)++;
	start = (*s);
	while (**s && **s != '\'')
		(*s)++;
	if (!**s)
	{
		printf("Single Quote error\n");
		return (-1);
	}
	*quote = 0;
	add_to_tok(ft_strndup(start, *s - start), t, i, TOKEN_WORD);
	(*s)++;
	return (1);
}

int	tok_check_doublequote(char **s, t_token **t, t_token **i, char *quote)
{
	if (**s != '"')
		return (0);
	printf("tok check double quote called, S : %s\n", *s);
	*quote = '"';
	(*s)++;
	while (**s && **s != '"')
	{
		if (tok_check_dq_env(s, t, i) > 0)
			continue;
		tok_check_dq_noenv(s, t, i);
	}
	if (!**s)
	{
		printf("Double Quote error\n");
		return (-1);
	}
	*quote = 0;
	(*s)++;
	return (1);
}

int	tok_check_dq_env(char **s, t_token **t, t_token **i)
{
	// char	*start;
	char	*env;

	if (**s != '$')
		return (0);
	printf("tok check double quote env called, S : %s\n", *s);
	(*s)++;
	// start = *s;
	if (special_expand_dq(s, t, i))
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env = ft_strdup("DOLLAR");
	if (env)
		add_to_tok(env, t, i, TOKEN_WORD);
	else
		add_to_tok(ft_strdup(""), t, i, TOKEN_WORD);
	return (1);
}

int	special_expand_dq(char **s, t_token **t, t_token **i)
{
	if (**s == '?')
	{
		if (add_to_tok(ft_itoa(56), t, i, TOKEN_WORD) == ERR_MALLOC)
			return (-1);
		(*s)++;
		return (1);
	}
	else if (ft_isspecial(**s, ENV_SPECIALS))
	{
		(*s)++;
		return (1);
	}
	else if (!ft_isalnum(**s) && **s != '_')
	{
		if (add_to_tok(ft_strdup("$"), t, i, TOKEN_WORD) == ERR_MALLOC)
			return (-1);
		return (1);
	}
	return (0);
}

int	tok_check_dq_noenv(char **s, t_token **t, t_token **i)
{
	char	*start;

	start = *s;
	while (**s && **s != '$' && **s != '"')
		(*s)++;
	add_to_tok(ft_strndup(start, *s - start), t, i, TOKEN_WORD);
	// if (*iter == NULL)
	// {
	// 	printf("Malloc error\n");
	// 	return (-1);
	// }
	return (1);
}

// int	tok_check_noquote(char **s, t_token **tok, t_token **iter)
// {
// 	while (**s && **s != '"' && **s != '\'')
// 	{
// 		if (tok_check_nq_env(s, tok, iter, mnsh) > 0)
// 			continue;
// 		tok_check_noquote_noenv(s, tok, iter);
// 	}
// 	return (1);
// }

// int	tok_check_nq_env(char **s, t_token **t, t_token **i, char *quote)
// {
// 	char	*start;
// 	char	*env;

// 	if (**s != '$')
// 		return (0);
// 	printf("tok check double quote env called, S : %s\n", *s);
// 	(*s)++;
// 	start = *s;
// 	if (special_expand_dq(s, t, i))
// 		return (1);
// 	while (**s && (ft_isalnum(**s) || **s == '_'))
// 		(*s)++;
// 	env = ft_strdup("DOLLAR");
// 	if (env)
// 		add_to_tok(env, t, i, TOKEN_WORD);
// 	else
// 		add_to_tok(ft_strdup(""), t, i, TOKEN_WORD);
// 	return (1);
// }

int main()
{
	char	*str = "'bonjour t'a\"\"\"biloute\"zert'out le monde'";

	printf("CMD : %s\n", str);
	tok_check_regular(&str);

	return (0);
}
