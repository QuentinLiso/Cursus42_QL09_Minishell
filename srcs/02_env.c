/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:41:56 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:52:25 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*alloc_env_var(char *key_eq_val)
{
	int		i;
	t_var	*var;

	var = ft_calloc(1, sizeof(t_var));
	if (!var)
		return (NULL);
	i = 0;
	while (key_eq_val[i] && key_eq_val[i] != '=')
		i++;
	var->key = ft_substr(key_eq_val, 0, i);
	if (!var->key)
		return (NULL);
	var->value = ft_strdup(&key_eq_val[i + 1]);
	if (!var->value)
		return (NULL);
	return (var);
}

char	*get_env_var(t_list *env_lst, char *key)
{
	t_var	*var;

	if (!env_lst)
		return (NULL);
	while (env_lst)
	{
		var = (t_var *)env_lst->content;
		if (ft_strcmp(var->key, key) == 0)
			return (var->value);
		env_lst = env_lst->next;
	}
	return (NULL);
}

t_list	*get_env_var_prev(t_list *env_lst, char *key)
{
	t_var	*var;

	if (!env_lst)
		return (NULL);
	while (env_lst->next)
	{
		var = (t_var *)env_lst->content;
		if (ft_strcmp(var->key, key) == 0)
			return (env_lst);
		env_lst = env_lst->next;
	}
	return (NULL);
}

int	add_env_var(t_list **env_lst, char *key, char *value)
{
	t_var	*var;
	t_list	*new_elem;

	var = ft_calloc(1, sizeof(t_var));
	if (!var)
		return (ENOMEM);
	var->key = ft_strdup(key);
	if (var->key && value)
		var->value = ft_strdup(value);
	if (!var->key || (value && !var->value))
		return (free_env_var_ret(var, ENOMEM));
	new_elem = ft_lstnew(var);
	if (!new_elem)
		return (free_env_var_ret(var, ENOMEM));
	ft_lstadd_back(env_lst, new_elem);
	return (0);
}

int	edit_env_var(t_list **env_lst, char *key, char *value)
{
	t_var	*var;
	t_list	*iterator;

	iterator = *env_lst;
	while (iterator)
	{
		var = (t_var *)iterator->content;
		if (!ft_strcmp(var->key, key))
		{
			if (value)
			{
				free_str(&var->value);
				var->value = ft_strdup(value);
				if (!var->value)
					return (ENOMEM);
			}
			return (0);
		}
		iterator = iterator->next;
	}
	return (add_env_var(env_lst, key, value));
}
