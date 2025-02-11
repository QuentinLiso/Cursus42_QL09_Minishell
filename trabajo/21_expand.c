#include "minishell.h"

void	expand_tok_mnsh(t_token **tokis, t_mnsh *mnsh)
{
	t_token	*iterator;
	t_token	*sub_tok;
	t_token	*new_tok;
	// t_token	*new_iter;
	(void)mnsh;
	iterator = *tokis;
	new_tok = NULL;
	while (iterator)
	{
		printf("-----------------------------\n");
		if (iterator->type != TOKEN_WORD)
		{
			iterator = iterator->next;
			continue;
		}
		printf("ITERATOR : %s\n", iterator->word);
		sub_tok = strtok_subtok(iterator->word, mnsh);
		printf("SUBTOK : \n");
		print_tokis(sub_tok);
		// appd_sub_tok(&new_tok, &new_iter, sub_tok);
		// ft_free_str(&iterator->word);
		//iterator->word = tok_strcat(sub_tokis);
		iterator = iterator->next;
	}
	// ft_free_all_tok(tokis);
	tokis = &new_tok;
}

void	appd_sub_tok(t_token **new_tok, t_token **new_iter, t_token *sub_tok)
{
	while (sub_tok)
	{
		printf("NEW_TOK : \n");
		print_tokis(*new_tok);
		if (sub_tok->word)
			add_to_tok(sub_tok->word, new_tok, new_iter, TOKEN_WORD);
		sub_tok = sub_tok->next;
	}
}

t_token	*strtok_subtok(char *s, t_mnsh *mnsh)
{
	t_token	*subtokis;
	t_token	*iterator;

	subtokis = NULL;
	iterator = NULL;
	while (*s)
	{
		if (tok_check_singlequote(&s, &subtokis, &iterator) > 0)
			continue;
		if (tok_check_doublequote(&s, &subtokis, &iterator, mnsh) > 0)
			continue;
		tok_check_noquote(&s, &subtokis, &iterator, mnsh);
	}
	return (subtokis);
}

int	tok_check_singlequote(char **s, t_token **tok, t_token **iter)
{
	char	*start;

	if (**s != '\'')
		return (0);
	(*s)++;
	start = (*s);
	while (**s && **s != '\'')
		(*s)++;
	add_to_tok(ft_strndup(start, *s - start), tok, iter, TOKEN_WORD);
	// if (**s == '\0')
	// {
	// 	printf("Single Quote error\n");
	// 	return (-1);
	// }
	// if ((*iter) == NULL)
	// {
	// 	printf("Malloc err\n");
	// 	return (-1);
	// }
	if (**s)
		(*s)++;
	return (1);
}

int	tok_check_doublequote(char **s, t_token **tok, t_token **iter, t_mnsh *mnsh)
{
	if (**s != '"')
		return (0);
	(*s)++;
	while (**s && **s != '"')
	{
		if (tok_check_env_dq(s, tok, iter, mnsh) > 0)
			continue;
		tok_check_dquote_noenv(s, tok, iter);
	}
	// if (**s == '\0')
	// {
	// 	printf("DQuote error\n");
	// 	return (-1);
	// }
	// if ((*iter) == NULL)
	// {
	// 	printf("Malloc err\n");
	// 	return (-1);
	// }
	if (**s)
		(*s)++;
	return (1);
}

int	tok_check_env_dq(char **s, t_token **tok, t_token **it, t_mnsh *mnsh)
{
	char	*start;
	char	*env;

	if (**s != '$')
		return (0);
	(*s)++;
	start = *s;
	if (special_expand(s, tok, it, true, mnsh))
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env = expanded_env(start, *s, mnsh);
	if (env)
		add_to_tok(expanded_env(start, *s, mnsh), tok, it, TOKEN_WORD);
	else
	{
		add_to_tok(ft_strdup(""), tok, it, TOKEN_WORD);
	}
		
	// if (*it == NULL)
	// {
	// 	printf("Malloc error\n");
	// 	return (-1);
	// }
	return (1);
}

int	tok_check_noquote(char **s, t_token **tok, t_token **iter, t_mnsh *mnsh)
{
	while (**s && **s != '"' && **s != '\'')
	{
		if (tok_check_env_nq(s, tok, iter, mnsh) > 0)
			continue;
		tok_check_noquote_noenv(s, tok, iter);
	}
	return (1);
}

int	tok_check_env_nq(char **s, t_token **tok, t_token **it, t_mnsh *mnsh)
{
	char	*start;
	char	*env;

	if (**s != '$')
		return (0);
	(*s)++;
	start = *s;
	if (special_expand(s, tok, it, false, mnsh))
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env = expanded_env(start, *s, mnsh);
	if(env)
		add_to_tok(env, tok, it, TOKEN_WORD);
	
	// if (*it == NULL)
	// {
	// 	printf("Malloc error\n");
	// 	return (-1);
	// }
	return (1);
}


int	special_expand(char **s, t_token **tok, t_token **it, bool q, t_mnsh *mnsh)
{
	if (**s == '?')
	{
		printf("A\n");
		if (add_to_tok(ft_itoa(mnsh->last_exit_status), tok, it, TOKEN_WORD)
			== ERR_MALLOC)
			return (-1);
		(*s)++;
		return (1);
	}
	else if (ft_isspecial(**s, ENV_SPECIALS) || (q == false && isquote(**s)))
	{
		printf("B\n");
		(*s)++;
		return (1);
	}
	else if (!ft_isalnum(**s) && **s != '_')
	{
		printf("C\n");
		if (add_to_tok(ft_strdup("$"), tok, it, TOKEN_WORD) == ERR_MALLOC)
			return (-1);
		return (1);
	}
	return (0);
}


int	tok_check_dquote_noenv(char **s, t_token **tok, t_token **iter)
{
	char	*start;

	start = *s;
	while (**s && **s != '$' && **s != '"')
		(*s)++;
	add_to_tok(ft_strndup(start, *s - start), tok, iter, TOKEN_WORD);
	// if (*iter == NULL)
	// {
	// 	printf("Malloc error\n");
	// 	return (-1);
	// }
	return (1);
}

int	tok_check_noquote_noenv(char **s, t_token **tok, t_token **iter)
{
	char	*start;

	start = *s;
	while (**s && **s != '$' && **s != '"' && **s != '\'')
		(*s)++;
	add_to_tok(ft_strndup(start, *s - start), tok, iter, TOKEN_WORD);
	// if (*iter == NULL)
	// {
	// 	printf("Malloc error\n");
	// 	return (-1);
	// }
	return (1);
}

char	*tok_strcat(t_token *tok)
{
	t_token *iterator;
	char	*d;
	size_t	len;
	int		ij[2];

	len = tok_strcat_len(tok);
	d = ft_calloc(len + 1, sizeof(char));
	if (!d)
		return (NULL);
	iterator = tok;
	ij[0] = 0;
	while (iterator)
	{
		ij[1] = 0;
		while (iterator->word[ij[1]])
			d[ij[0]++] = iterator->word[ij[1]++];
		iterator = iterator->next;
	}
	d[ij[0]] = '\0';
	ft_free_all_tok(&tok);
	return (d);
}

size_t	tok_strcat_len(t_token *tok)
{
	size_t	len;
	t_token	*iterator;

	len = 0;
	iterator = tok;
	while (iterator)
	{
		len += ft_strlen(iterator->word);
		iterator = iterator->next;
	}
	return (len);
}
