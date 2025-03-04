/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_env2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:43:37 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:28 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_env_var(void *ptr)
{
	t_var	*env_var;

	if (!ptr)
		return ;
	env_var = (t_var *)ptr;
	ft_putstr_fd(env_var->key, STDOUT_FILENO);
	ft_putchar_fd('=', STDOUT_FILENO);
	ft_putendl_fd(env_var->value, STDOUT_FILENO);
}
