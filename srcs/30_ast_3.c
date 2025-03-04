/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   30_ast_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:57:09 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 12:10:48 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_heredoc_name(char **heredoc_name)
{
	static int	count = 0;
	int			attempts;
	int			status;

	attempts = 0;
	status = 0;
	while (attempts < 10000)
	{
		status = check_valid_heredoc_name(heredoc_name, &count, &attempts);
		if (status == 0)
			return (0);
		else if (status > 0)
			continue ;
		else if (status < 0)
			return (-status);
	}
	return (1);
}

int	check_valid_heredoc_name(char **heredoc_name, int *count, int *attempts)
{
	struct stat	st;
	char		*count_itoa;

	attempts++;
	(*count)++;
	if (*count < 0)
		*count = 1;
	count_itoa = ft_itoa(*count);
	if (!count_itoa)
		return (-ENOMEM);
	*heredoc_name = ft_strjoin("mnsh-hdoc-tmp-", count_itoa);
	free_str(&count_itoa);
	if (!*heredoc_name)
		return (-ENOMEM);
	if (!stat(*heredoc_name, &st))
	{
		free_str(heredoc_name);
		return (1);
	}
	else
		return (0);
}

int	create_heredoc(t_redir *redir_file, char *heredoc_end, t_mnsh *mnsh)
{
	int		fd_write;
	int		status;

	fd_write = open(redir_file->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_write < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	redir_file->fd_heredoc_read = open(redir_file->file, O_RDONLY);
	if (redir_file->fd_heredoc_read < 0)
	{
		close(fd_write);
		unlink(redir_file->file);
		return (perror_mnsh(1, 1, strerror(errno)));
	}
	if (unlink(redir_file->file) < 0)
		perror_mnsh(1, 1, strerror(errno));
	status = fill_heredoc(fd_write, heredoc_end, mnsh);
	close (fd_write);
	if (status)
		close(redir_file->fd_heredoc_read);
	return (status);
}

int	fill_heredoc(int fd_wr, char *heredoc_end, t_mnsh *mnsh)
{
	int		count_pipe[2];
	pid_t	pid;

	if (pipe(count_pipe) < 0)
		return (perror_mnsh(1, 1, "pipe failed"));
	pid = fork();
	if (pid < 0)
		return (perror_mnsh(1, 1, "fork failed"));
	else if (pid == 0)
		exit(child_heredoc(fd_wr, count_pipe, heredoc_end, mnsh));
	else
		return (parent_heredoc(count_pipe, pid, mnsh));
}

int	child_heredoc(int fd_wr, int count_pipe[2], char *h_end, t_mnsh *mnsh)
{
	int		count;
	int		status;

	close(count_pipe[0]);
	set_sigaction(&mnsh->sa_sigint, SIGINT, h_sigint_heredoc, 0);
	count = 0;
	status = loop_heredoc(fd_wr, &count, h_end, mnsh);
	g_signal_received = 0;
	write(count_pipe[1], &count, sizeof(int));
	close(count_pipe[1]);
	free_all_mnsh(mnsh);
	return (status);
}
