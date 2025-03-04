/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:13:49 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 12:22:48 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_ast_op_pipe(t_ast **node, t_mnsh *mnsh)
{
	int		fd[2];
	pid_t	pid[2];
	int		status[2];

	if (pipe(fd) < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	if (left_pipe(node, &fd, &pid[0], mnsh))
		return (1);
	if (right_pipe(node, &fd, &pid[1], mnsh))
		return (1);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);
	if (status[1])
		return (1);
	return (0);
}

int	left_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh)
{
	int	child_exit;

	*pid = fork();
	if (*pid < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	else if (*pid == 0)
	{
		close((*fd)[0]);
		if (dup2((*fd)[1], STDOUT_FILENO) < 0)
			exit(perror_mnsh(1, 1, strerror(errno)));
		close((*fd)[1]);
		child_exit = execute_ast(&(*node)->left_node, mnsh);
		free_all_mnsh(mnsh);
		exit(child_exit);
	}
	return (0);
}

int	right_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh)
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
