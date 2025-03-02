/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:13:49 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 16:14:12 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		right_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh)
{
	int	child_exit;

	*pid = fork();
	if (*pid < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	else if (*pid == 0)
	{
		close((*fd)[1]);
		if (dup2((*fd)[0], STDIN_FILENO) < 0)
			exit(perror_mnsh(1, 1, strerror(errno)));
		close((*fd)[0]);
		child_exit = execute_ast(&(*node)->right_node, mnsh);
		free_all_mnsh(mnsh);
		exit(child_exit);
	}
	return (0);
}