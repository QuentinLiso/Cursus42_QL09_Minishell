/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   70_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:17:27 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 17:17:55 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **env_lst_to_arr(t_list *env_list)
{
	char	**arr;
	t_var	*env_var;
    int len;
    int i;

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
