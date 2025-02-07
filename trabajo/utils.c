#include "minishell.h"

t_error	mnsh_perror(t_error	error)
{
	if (error == ERR_ARGS)
		ft_putstr_fd(ERR_ARGS_MSG, STDERR_FILENO);
	else if (error == ERR_ENV)
		ft_putstr_fd(ERR_ENV_MSG, STDERR_FILENO);
	else if (error == ERR_QUOTE)
		ft_putstr_fd(ERR_QUOTE_MSG, STDERR_FILENO);
		
	return (error);
}

t_error	perror_malloc(char *func_name)
{
	ft_putstr_fd("Malloc error in function ", STDERR_FILENO);
	ft_putstr_fd(func_name, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (ERR_MALLOC);
}

bool	ft_isspace(char c)
{
	return (c == '\f' || c == '\t' || c == '\n' || c == '\r'
			|| c == '\v' || c == ' ');
}

bool	ft_isspecial(char c, const char *list_specials)
{
	char *specials;

	specials = (char *)list_specials;
	while (*specials)
	{
		if (c == *specials)
			return (true);
		specials++;
	}
	return (false);
}

bool	ft_str_contain(char *s, char c)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (true);
	return (false);
}

void	ft_free_str(char **ptr)
{
	if (*ptr == NULL)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void		ft_free_strarray(char ***arr)
{
	int		i;

	i = -1;
	if ((*arr) == NULL)
		return ;
	while ((*arr)[++i])
		ft_free_str(&(*arr)[i]);
	free(*arr);
}

t_error		ft_free_strarray_perror(char ***arr, t_error err)
{
	ft_free_strarray(arr);
	return (err);
}

int		is_operator(const char *s, const char *list_operators)
{
	char	**operators;
	int		i;
	size_t	len;

	operators = ft_split(list_operators, ' ');
	i = -1;
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

int		is_indir(const char *s)
{
	char	**indir;
	int		i;
	size_t	len;

	indir = ft_split(INDIR, ' ');
	i = -1;
	while (indir[++i])
	{
		len = ft_strlen(indir[i]);
		if (ft_strncmp(s, indir[i], len) == 0)
		{
			ft_free_strarray(&indir);
			return (len);
		}
	}
	ft_free_strarray(&indir);	
	return (0);
}

size_t	ft_strlenchar(char *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

char	*ft_strndup(const char *s, int n)
{
	char	*d;
	int		i;

	if (!s || n < 0)
		return (NULL);
	d = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!d)
		return (NULL);
	i = -1;
	while (s[++i] && i < n)
		d[i] = s[i];
	d[i] = '\0';
	return (d);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	ft_strarrlen(char **arr)
{
	int	len;

	if (!arr)
		return (0);
	len = 0;
	while (arr[len])
		len++;
	return (len);
}

char	*ft_strjoin_multi(int count, ...)
{
	va_list	args;
	size_t	len;
    char    *dest;

	va_start(args, count);
	len = ft_strjoin_multi_getlen(count, args);
    va_end(args);
    va_start(args, count);
    dest = ft_strjoin_multi_setstr(len, count, args);
    va_end(args);
    return (dest);
}

size_t  ft_strjoin_multi_getlen(int count, va_list args)
{
    int     i;
    size_t  len;

    i = -1;
    len = 0;
    while (++i < count)
        len += ft_strlen(va_arg(args, char *));
    return (len);
}

char    *ft_strjoin_multi_setstr(size_t len, int count, va_list args)
{
    char    *dest;
    char    *buf;
    int     i;
    int     j;

    dest = ft_calloc(len + 1, sizeof(char));
    if (!dest)
        return (NULL);
    j = 0;
    while (count--)
    {
        i = -1;
        buf = va_arg(args, char *);
        while (buf[++i])
            dest[j++] = buf[i];
    }
    dest[j] = '\0';
    return (dest);
}