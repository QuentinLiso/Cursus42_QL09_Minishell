/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   50_builtins_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:19:27 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 12:24:28 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mnsh_unset(char **args, t_mnsh *mnsh)
{
	int		i;

	i = 0;
	while (args[++i])
		del_node(&mnsh->env_mnsh_lst, args[i]);
	return (0);
}

void	del_node(t_list **list, char *key)
{
	t_list	*iter;
	t_list	*prev;
	t_var	*var;

	if (!list || !*list || !key)
		return ;
	iter = *list;
	prev = NULL;
	while (iter)
	{
		var = (t_var *)iter->content;
		if (!ft_strcmp(var->key, key))
			break ;
		prev = iter;
		iter = iter->next;
	}
	if (!iter)
		return ;
	if (!prev)
		*list = iter->next;
	else
		prev->next = iter->next;
	ft_lstdelone(iter, &free_env_var);
}

int	mnsh_exit(char **args, t_mnsh *mnsh)
{
	long long		exit_code;
	bool			is_llong_num;

	exit_code = 0;
	if (args[1] == NULL)
		exit_code = mnsh->last_exit_status;
	else
	{
		is_llong_num = strtoll_isnum(args[1], &exit_code);
		if (!is_llong_num)
			exit_code = perror_mnsh(2, 3, "exit", args[1],
					"numeric argument required");
		else if (is_llong_num && args[2])
			return (perror_mnsh(errno_to_exit(E2BIG), 2, "exit",
					"too many arguments"));
	}
	free_all_mnsh(mnsh);
	mnsh->last_exit_status = exit_code % 256;
	exit(mnsh->last_exit_status);
	return (0);
}

bool	strtoll_isnum(char *str, long long *n)
{
	int	sign;
	int	digit;

	*n = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
		sign = ',' - *str++;
	if (!*str)
		return (false);
	while (*str >= '0' && *str <= '9')
	{
		digit = *str++ - '0';
		if ((sign == 1 && (*n > (LLONG_MAX - digit) / 10))
			|| (sign == -1 && (*n * sign < (LLONG_MIN + digit) / 10)))
			return (false);
		*n = *n * 10 + digit;
	}
	if (*str)
		return (false);
	*n *= sign;
	return (true);
}
