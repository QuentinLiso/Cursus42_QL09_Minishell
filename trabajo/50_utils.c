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

bool	isquote(char c)
{
	return (c == '\'' || c == '"');
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

char	*ft_strappend_mnsh(char *s1, char *s2)
{
	char	*d;
	size_t	len1;
	size_t	len2;

	if (!s1 && s2)
		return (ft_strdup(s2));
	if (!s1 && !s2)
		return (NULL);
	if (s1 && !s2)
		return (s1);
	len1 = ft_strlen_gnl(s1);
	len2 = ft_strlen_gnl(s2);
	d = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!d)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(d, s1, len1);
	free(s1);
	ft_memcpy(d + len1, s2, len2 + 1);
	return (d);
}

void	ft_perror_mnsh(char *cmd, char *errmsg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(errmsg, STDERR_FILENO);
}

void	ft_perror_exit_mnsh(char *cmd, char *errmsg, int errexit, char **exe)
{
	int	exit_code;

	exit_code = errexit;
	ft_perror_mnsh(cmd, errmsg);
	if (exe && *exe)
		ft_free_str(exe);
	exit(exit_code);
}