/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   70_utils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:16:40 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 12:25:32 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	load_message(int size, char *msg, int speed)
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

int	errno_to_exit(int errnum)
{
	if (errnum == ENOMEM)
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

int	perror_mnsh(int errnum, int count, ...)
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

int	perror2_mnsh(int errnum, int count, ...)
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
