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

void	print_strarray(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	ft_printf("Strarray : [");
	while (arr[++i])
		ft_printf("\"%s\", ", arr[i - 1]);
	ft_printf("\"%s\"]\n", arr[i - 1]);
}

bool	ft_isspace(char c)
{
	return (c == '\f' || c == '\t' || c == '\n' || c == '\r'
			|| c == '\v' || c == ' ');
}

bool	ft_isspecial(char c)
{
	char *specials;

	specials = SPECIALS;
	while (*specials)
	{
		if (c == *specials)
			return (true);
		specials++;
	}
	return (false);
}

void	free_strarray(char ***arr)
{
	int		i;
	char	**buf;

	buf = *arr;
	i = -1;
	while (buf[++i])
	{
		free(buf[i]);
	}	
	free(buf);
}

int		is_operator(const char *s)
{
	char	**operators;
	int		i;
	size_t	len;

	operators = ft_split(OPERATORS, ' ');
	i = -1;
	while (operators[++i])
	{
		len = ft_strlen(operators[i]);
		if (ft_strncmp(s, operators[i], len) == 0)
		{
			free_strarray(&operators);
			return (len);
		}
	}
	free_strarray(&operators);	
	return (0);
}

char	*ft_strndup(const char *s, int n)
{
	char	*d;
	int		i;

	if (!s || n < 0)
		return (NULL);
	d = ft_calloc(ft_strlen(s), sizeof(char));
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
