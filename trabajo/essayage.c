#include "minishell.h"

void	regular_split(char *s);
bool	still_regular(char c, char quote);
int		split_quote(char **s, char **buffer);
int		split_dquote(char **s, char **buffer);
int		split_dquote_env(char **s, char **dup_buf);
int		split_dquote_env_spec(char **s, char **dup_buf);
int		split_dquote_noenv(char **s, char **dup_buf);
int		split_noquote(char **s, char **buffer);
int		split_noquote_env(char **s, char **dup_buf);
int		split_noquote_env_spec(char **s, char **dup_buf);
int		split_noquote_noenv(char **s, char **dup_buf);

void	regular_split(char *s)
{
	char	quote;
	char	*buffer;

	quote = 0;
	buffer = NULL;
	while (*s && still_regular(*s, quote))
	{
		printf("S : %s -> ", s);
		if (buffer && *buffer)
			printf("New s : %s\n", buffer);
		else if (buffer && *buffer)
			printf("New s : (empty str)\n");
		else
			printf("New s : (null)\n");		
		if (split_quote(&s, &buffer) > 0)
			continue;
		if (split_dquote(&s, &buffer) > 0)
			continue;
		if (split_noquote(&s, &buffer) > 0)
			continue;
	}
	printf("S : %s -> ", s);
	if (buffer)
		printf("New s : %s\n", buffer);
	else
		printf("New s : (null)\n");	
	free(buffer);
}

bool	still_regular(char c, char quote)
{
	if (quote)
		return (true);
	else if (!quote && (ft_isspace(c) || ft_isspecial(c, TOK_SPECIALS)))
		return (false);
	else
		return (true);
}

int		split_quote(char **s, char **buffer)
{
	char	*dup_buf;
	char	*start;

	if (**s != '\'')
		return (0);
	printf("split quote called\n");
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

int		split_dquote(char **s, char **buffer)
{
	char	*dup_buf;

	if (**s != '"')
		return (0);
	printf("split dquote called\n");
	dup_buf = NULL;
	(*s)++;
	while (**s && **s != '"')
	{
		if (split_dquote_env(s, &dup_buf) > 0)
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

int		split_dquote_env(char **s, char **dup_buf)
{
	char	*start;
	char	*env;

	if (**s != '$')
		return (0);
	(*s)++;
	start = (*s);
	if (split_dquote_env_spec(s, dup_buf) > 0)
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env = ft_strndup(start, *s - start);
	if (!env)
		return (-1);
	*dup_buf = ft_strappend_mnsh(*dup_buf, ft_strdup(env));
	ft_free_str(&env);
	return (1);
}

int		split_dquote_env_spec(char **s, char **dup_buf)
{
	char	*dup2_buf;

	if (**s == '?')
	{
		dup2_buf = ft_itoa(56);
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

int		split_noquote(char **s, char **buffer)
{
	char	*dup_buf;

	printf("split noquote called\n");
	dup_buf = NULL;
	while (**s && **s != '"' && **s != '\'')
	{
		if (split_noquote_env(s, &dup_buf) > 0)
			continue ;
		split_noquote_noenv(s, &dup_buf);
	}
	if (dup_buf)
		*buffer = ft_strappend_mnsh(*buffer, dup_buf);
	ft_free_str(&dup_buf);
	return (1);
}

int		split_noquote_env(char **s, char **dup_buf)
{
	char	*start;
	char	*env;

	if (**s != '$')
		return (0);
	(*s)++;
	start = (*s);
	if (split_noquote_env_spec(s, dup_buf) > 0)
		return (1);
	while (**s && (ft_isalnum(**s) || **s == '_'))
		(*s)++;
	env = ft_strndup(start, *s - start);
	if (!env)
		return (-1);
	*dup_buf = ft_strappend_mnsh(*dup_buf, ft_strdup(env));
	ft_free_str(&env);
	return (1);
}

int		split_noquote_env_spec(char **s, char **dup_buf)
{
	char	*dup2_buf;

	if (**s == '?')
	{
		dup2_buf = ft_itoa(56);
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
	else if (!ft_isalnum(**s) && **s != '_')
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
	while (**s && **s != '$' && **s != '"' && **s != '\'')
		(*s)++;
	dup2_buf = ft_strndup(start, *s - start);
	*dup_buf = ft_strappend_mnsh(*dup_buf, dup2_buf);
	ft_free_str(&dup2_buf);
	return (1);
}







int main()
{
	char	*str = "\"$\" salut";

	printf("CMD : %s\n", str);
	printf("======================\n");
	regular_split(str);

	return (0);
}
