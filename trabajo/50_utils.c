#include "minishell.h"

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

int		load_message(int size, char *msg, int speed)
{
	int	i;
	int	j;

	i = -1;
	while (++i <= size)
	{
		j = -1;
		ft_putstr_fd("\033[32m   [", STDOUT_FILENO);
		while (++j < i)
			ft_putchar_fd('#', STDOUT_FILENO);
		while (++j <= size)
		ft_putchar_fd('.', STDOUT_FILENO);
		ft_putstr_fd("] ", STDOUT_FILENO);
		ft_putnbr_fd(((float)i / (float)size) * 100, STDOUT_FILENO);
		ft_putstr_fd("%\t", STDOUT_FILENO);
		if (i < size)
			ft_putchar_fd('\r', STDOUT_FILENO);
		usleep(speed);
	}
	ft_putstr_fd("\033[0m\033[0;35m  ", STDOUT_FILENO);
	ft_putendl_fd(msg, STDOUT_FILENO);
	ft_putstr_fd("\033[0m", STDOUT_FILENO);
	return (0);
}

int		errno_to_exit(int errnum)
{
	if(errnum == ENOMEM)
		return (12);
	else if (errnum == EACCES || errnum == EISDIR)
		return (126);
	else if (errnum == ENOENT)
		return (127);
	else if (errnum == 0)
		return (0);
	else
		return (1);

}

int		perror_mnsh(int errnum, int count, ...)
{
	va_list	args;

	va_start(args, count);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	while (--count)
	{
		ft_putstr_fd(va_arg(args, char *), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(va_arg(args, char *), STDERR_FILENO);
	va_end(args);
	return (errnum);
}

int		perror2_mnsh(int errnum, int count, ...)
{
	va_list	args;

	va_start(args, count);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	while (--count)
		ft_putstr_fd(va_arg(args, char *), STDERR_FILENO);
	ft_putendl_fd(va_arg(args, char *), STDERR_FILENO);
	va_end(args);
	return (errnum);
}

char    **lst_to_arr(t_list *list)
{
	char	**arr;
    int len;
    int i;

    len = ft_lstsize(list);
    arr = ft_calloc(len + 1, sizeof(char *));
    i = -1;
    while (++i < len)
	{
		arr[i] = ft_strdup((char *)list->content);
		list = list->next;
		if (!arr[i])
		{
			ft_free_strarray(&arr);
			return (NULL);
		}
	}
    arr[i] = NULL;
    return (arr);
}

