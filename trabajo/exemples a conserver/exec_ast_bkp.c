#include "minishell.h"


//==========================================================================

void	expand_env_vars(char ***args, t_mnsh *mnsh)
{
	int	i;
	char	**tokens;
	// char	*buf;

	tokens = ft_calloc(128, sizeof(char *));

	i = -1;
	while((*args)[++i])
	{
		if ((*args)[i][0] == '\'')
		{
			// buf = &(*args)[i][1];
			// ft_free_str(&(*args)[i]);
			// (*args)[i] = buf;
			return ;
		}
		if (strtok_arg_env((*args)[i], &tokens, mnsh))
			return ;
		free((*args)[i]);
		(*args)[i] = ft_strcat_arr(tokens);
	}
	ft_free_strarray(&tokens);
}

t_error strtok_arg_env(char *s, char ***tokens, t_mnsh *mnsh)
{
    int     i;
    t_error tok_status;

    i = 0;
    while (*s)
    {
        tok_status = env_check(&s, tokens, &i, mnsh);
        if (tok_status == CONTINUE)
            continue;
        else if (tok_status > 0)
            return (tok_status);
    }
    (*tokens)[i] = NULL;
    return (ERR_NOERR);
}

t_error env_check(char **s, char ***tokens, int *i, t_mnsh *mnsh)
{
    t_error status;

    status = env_check_dollar(s, tokens, i, mnsh);
    if (status)
        return (status);
    status = env_check_regular(s, tokens, i);
    if (status)
        return (status);
    return (status);
}

t_error env_check_dollar(char **s, char ***tokens, int *i, t_mnsh *mnsh)
{
    char    *start;

    if (**s == '$')
    {
        start = *s;
        (*s)++;
        if (**s == '?')
        {
            (*s)++;
            (*tokens)[*i] = ft_itoa(mnsh->last_exit_status);
            if ((*tokens)[*i] == NULL)
                return (perror_malloc("env_check_dollar"));
            (*i)++;
            return (CONTINUE);
        }
        while (**s && (ft_isalnum(**s) || **s == '_'))
            (*s)++;
        (*tokens)[*i] = expand_env_var(start, *s, mnsh);
        if ((*tokens)[*i] == NULL)
            return (perror_malloc("env_check_dollar"));
        (*i)++;
        return (CONTINUE);
    }
    return (ERR_NOERR);
}

char    *expand_env_var(char *start, char *s, t_mnsh *mnsh)
{
    char    *env;
    char    *dup_buffer;
	
    dup_buffer = ft_strndup(start + 1, s - start - 1);
	env = ft_get_env_var(mnsh->env_mnsh, dup_buffer);
	free(dup_buffer);
	if (!env)
		return (NULL);
    return (env);
}

t_error env_check_regular(char **s, char ***tokens, int *i)
{
    char    *start;

    start = *s;
    while (**s && **s != '$')
        (*s)++;
    (*tokens)[*i] = ft_strndup(start, *s - start);
    if ((*tokens)[*i] == NULL)
        return (perror_malloc("env_check_regular"));
    (*i)++;
    return (CONTINUE);
}

char    *ft_strcat_arr(char **arr)
{
    size_t  len;
    int     iter[3];
    char    *d;

    len = 0;
    iter[0] = -1;
    while (arr[++iter[0]])
        len += ft_strlen(arr[iter[0]]);
    d = ft_calloc(len + 1, sizeof(char));
    if (!d)
        return (NULL);
    iter[0] = -1;
    iter[1] = 0;
    while (arr[++iter[0]])
    {
        iter[2] = 0;
        while (arr[iter[0]][iter[2]])
            d[iter[1]++] = arr[iter[0]][iter[2]++];
    }
    d[iter[1]] = 0;
    return (d);
}
