/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:07:19 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:42 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_indir_elem(t_redir *elem, int *fd_in, int *fd_out)
{
	if (!elem || !elem->file)
		return (0);
	if ((elem->style == REDIR_IN) || (elem->style == REDIR_HEREDOC))
		return (dup_indir_elem_in(elem, fd_in));
	else if (elem->style == REDIR_OUT)
		return (dup_indir_elem_out(elem->file, fd_out, O_TRUNC));
	else if (elem->style == REDIR_APPEND)
		return (dup_indir_elem_out(elem->file, fd_out, O_APPEND));
	return (0);
}

int	dup_indir_elem_in(t_redir *elem, int *fd_in)
{
	if (elem->style == REDIR_IN)
		*fd_in = open(elem->file, O_RDONLY);
	else
		*fd_in = elem->fd_heredoc_read;
	if (*fd_in < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	if (dup2(*fd_in, STDIN_FILENO) < 0)
	{
		close(*fd_in);
		return (perror_mnsh(1, 1, strerror(errno)));
	}
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = -1;
	return (0);
}

int	dup_indir_elem_out(char *file, int *fd_out, int flag)
{
	*fd_out = open(file, O_WRONLY | O_CREAT | flag, 0644);
	if (*fd_out < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	if (dup2(*fd_out, STDOUT_FILENO) < 0)
	{
		close(*fd_out);
		return (perror_mnsh(1, 1, strerror(errno)));
	}
	close(*fd_out);
	*fd_out = -1;
	return (0);
}

int	exec_ast_cmd_external(char **args, t_mnsh *mnsh)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror_mnsh(1, 1, "fork failed"));
	else if (pid == 0)
		exit(exec_ast_cmd_ext_child(args, mnsh));
	else
		return (exec_ast_cmd_ext_parent(pid, mnsh));
}

int	exec_ast_cmd_ext_child(char **args, t_mnsh *mnsh)
{
	int	child_exit;

	set_sigaction(&mnsh->sa_sigint, SIGINT, h_sigint_cmd, 0);
	set_sigaction(&mnsh->sa_sigquit, SIGQUIT, h_sigquit_cmd, 0);
	if (g_signal_received == SIGINT)
		child_exit = 130;
	else if (g_signal_received == SIGQUIT)
		child_exit = 131;
	g_signal_received = 0;
	child_exit = check_and_execute_cmd(args, mnsh);
	free_all_mnsh(mnsh);
	return (child_exit);
}
