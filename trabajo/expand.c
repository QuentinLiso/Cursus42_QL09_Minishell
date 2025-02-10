#include "minishell.h"

void	expand_tok_mnsh(t_token **tokis, t_mnsh *mnsh)
{
	t_token	*iterator;
	t_token	*sub_tokis;

	iterator = *tokis;
	while (iterator)
	{
		if (iterator->type != TOKEN_WORD)
		{
			iterator = iterator->next;
			continue;
		}
		sub_tokis = strtok_toktok(iterator->word, mnsh);
		ft_free_str(&iterator->word);
		iterator->word = tok_strcat(sub_tokis);
		iterator = iterator->next;
	}
}

t_token	*strtok_toktok(char *s, t_mnsh *mnsh)
{
	t_token	*toktokis;
	t_token	*iterator;

	toktokis = NULL;
	iterator = NULL;
	while (*s)
	{
		if (tok_check_singlequote(&s, &toktokis, &iterator) > 0)
			continue;
		if (tok_check_doublequote(&s, &toktokis, &iterator, mnsh) > 0)
			continue;
		tok_check_noquote(&s, &toktokis, &iterator, mnsh);
	}
	return (toktokis);
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
	if (**s == '\0')
	{
		printf("Single Quote error\n");
		return (-1);
	}
	if ((*iter) == NULL)
	{
		printf("Malloc err\n");
		return (-1);
	}
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
		if (tok_check_env(s, tok, iter, mnsh) > 0)
			continue;
		tok_check_dquote_noenv(s, tok, iter);
	}
	if (**s == '\0')
	{
		printf("DQuote error\n");
		return (-1);
	}
	if ((*iter) == NULL)
	{
		printf("Malloc err\n");
		return (-1);
	}
	if (**s)
		(*s)++;
	return (1);
}

int	tok_check_noquote(char **s, t_token **tok, t_token **iter, t_mnsh *mnsh)
{
	while (**s && **s != '"' && **s != '\'')
	{
		if (tok_check_env(s, tok, iter, mnsh) > 0)
			continue;
		tok_check_noquote_noenv(s, tok, iter);
	}
	if ((*iter) == NULL)
	{
		printf("Malloc err\n");
		return (-1);
	}
	return (1);
}

int	tok_check_env(char **s, t_token **tok, t_token **iter, t_mnsh *mnsh)
{
	char	*start;

	if (**s != '$')
		return (0);
	(*s)++;
	start = *s;
	if (special_expand(s, tok, iter, mnsh))
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	add_to_tok(expanded_env(start, *s, mnsh), tok, iter, TOKEN_WORD);
	if (*iter == NULL)
	{
		printf("Malloc error\n");
		return (-1);
	}
	return (1);
}

int	special_expand(char **s, t_token **tok, t_token **iter, t_mnsh *mnsh)
{
	if (**s == '?')
	{
		if (add_to_tok(ft_itoa(mnsh->last_exit_status), tok, iter, TOKEN_WORD)
			== ERR_MALLOC)
			return (-1);
		(*s)++;
		return (1);
	}
	else if (**s == '\0')
	{
		if (add_to_tok(ft_strdup("$"), tok, iter, TOKEN_WORD) == ERR_MALLOC)
			return (-1);
		(*s)++;
		return (1);
	}
	return (0);
}

char	*expanded_env(char *start, char *s, t_mnsh *mnsh)
{
	char    *env;
    char    *dup_buffer;
	
    dup_buffer = ft_strndup(start, s - start);
	env = ft_get_env_var(mnsh->env_mnsh, dup_buffer);
	free(dup_buffer);
	if (!env)
		return (NULL);
    return (env);
}

int	tok_check_dquote_noenv(char **s, t_token **tok, t_token **iter)
{
	char	*start;

	start = *s;
	while (**s && **s != '$' && **s != '"')
		(*s)++;
	add_to_tok(ft_strndup(start, *s - start), tok, iter, TOKEN_WORD);
	if (*iter == NULL)
	{
		printf("Malloc error\n");
		return (-1);
	}
	return (1);
}

int	tok_check_noquote_noenv(char **s, t_token **tok, t_token **iter)
{
	char	*start;

	start = *s;
	while (**s && **s != '$' && **s != '"' && **s != '\'')
		(*s)++;
	add_to_tok(ft_strndup(start, *s - start), tok, iter, TOKEN_WORD);
	if (*iter == NULL)
	{
		printf("Malloc error\n");
		return (-1);
	}
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

