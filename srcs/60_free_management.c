/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   60_free_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:23:47 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:53 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str(char **ptr)
{
	if (*ptr == NULL)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void	free_str_lst(void *ptr)
{
	char	*str;

	str = (char *)ptr;
	if (str == NULL)
		return ;
	free(str);
	str = NULL;
}

void	free_strarray(char ***arr)
{
	int		i;

	i = -1;
	if ((*arr) == NULL)
		return ;
	while ((*arr)[++i])
		free_str(&(*arr)[i]);
	free(*arr);
	(*arr) = NULL;
}

int	free_env_var_ret(t_var *var, int ret)
{
	if (var)
	{
		free_str(&var->key);
		free_str(&var->value);
		free(var);
	}
	return (ret);
}

void	free_env_var(void *ptr)
{
	t_var	*var;

	var = (t_var *)ptr;
	if (var)
	{
		free_str(&var->key);
		free_str(&var->value);
		free(var);
	}
}
