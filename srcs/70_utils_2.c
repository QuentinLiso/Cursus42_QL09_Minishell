/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   70_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:17:27 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:31:02 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_lst_to_arr(t_list *env_list)
{
	t_var	*env_var;
	char	**arr;
	int		len;
	int		i;

	len = ft_lstsize(env_list);
	arr = ft_calloc(len + 1, sizeof(char *));
	i = -1;
	while (++i < len)
	{
		env_var = (t_var *)env_list->content;
		arr[i] = env_var_to_char(env_var);
		env_list = env_list->next;
		if (!arr[i])
		{
			free_strarray(&arr);
			return (NULL);
		}
	}
	arr[i] = NULL;
	return (arr);
}

char	*env_var_to_char(t_var *var)
{
	char	*s;
	size_t	key_len;
	size_t	value_len;

	if (!var || !var->key)
		return (NULL);
	key_len = ft_strlen(var->key);
	value_len = ft_strlen(var->value);
	s = ft_calloc(key_len + value_len + 2, sizeof(char));
	if (!s)
		return (NULL);
	ft_memcpy(s, var->key, key_len);
	ft_memcpy(s + key_len, "=", 1);
	ft_memcpy(s + key_len + 1, var->value, value_len);
	s[key_len + value_len + 1] = '\0';
	return (s);
}

void	increment_mnsh_line_count(t_mnsh *mnsh, int i)
{
	while (i-- > 0)
	{
		mnsh->line_count++;
		if (mnsh->line_count < 0)
			mnsh->line_count = 1;
	}
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
		ft_putstr_fd("%%\t", STDOUT_FILENO);
		if (i < size)
			ft_putchar_fd('\r', STDOUT_FILENO);
		usleep(speed);
	}
	ft_putstr_fd("\033[0m\033[0;35m  ", STDOUT_FILENO);
	ft_putendl_fd(msg, STDOUT_FILENO);
	ft_putstr_fd("\033[0m", STDOUT_FILENO);
	return (0);
}
