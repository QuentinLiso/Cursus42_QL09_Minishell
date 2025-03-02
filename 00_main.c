/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:33:09 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 15:34:33 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	main(int ac, __attribute__((unused)) char **av, char **env)
{
	t_mnsh	mnsh;

	if (mnsh_initialization(&mnsh, ac, env))
	{
		free_all_mnsh(&mnsh);
		return (mnsh.last_exit_status);
	}
	loop_mnsh(&mnsh);
	free_all_mnsh(&mnsh);
	return (mnsh.last_exit_status);
}
