/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:06:15 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:41 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_ast_cmd_indir(t_list *redir)
{
	int	status;

	status = check_access_indir_lst(redir);
	if (status)
		return (status);
	status = dup_indir_lst(redir);
	if (status)
		return (status);
	return (0);
}

int	check_access_indir_lst(t_list *redir)
{
	t_redir	*elem;
	int		status;

	while (redir)
	{
		elem = (t_redir *)redir->content;
		status = check_access_indir_elem(elem);
		if (status)
			return (status);
		redir = redir->next;
	}
	return (0);
}

int	check_access_indir_elem(t_redir *elem)
{
	struct stat	st;

	if (elem && elem->file
		&& ((elem->style == REDIR_IN)))
	{
		if (access(elem->file, R_OK) < 0)
			return (perror_mnsh(1, 2, elem->file, strerror(errno)));
	}
	else if (elem && elem->file
		&& ((elem->style == REDIR_OUT) || (elem->style == REDIR_APPEND)))
	{
		if (!stat(elem->file, &st))
		{
			if (access(elem->file, W_OK) < 0)
				return (perror_mnsh(1, 2, elem->file, strerror(errno)));
			return (0);
		}
		return (create_outfile(elem));
	}
	return (0);
}

int	create_outfile(t_redir *redir_file)
{
	int	flag;
	int	fd;

	if (redir_file->style == REDIR_APPEND)
		flag = O_APPEND;
	else
		flag = O_TRUNC;
	fd = open(redir_file->file, O_WRONLY | O_CREAT | flag, 0644);
	if (fd < 0)
		return (perror_mnsh(errno_to_exit(errno), 2, redir_file->file,
				strerror(errno)));
	close(fd);
	return (0);
}

int	dup_indir_lst(t_list *redir)
{
	t_redir	*elem;
	int		status;
	int		fd_in;
	int		fd_out;

	fd_in = -1;
	fd_out = -1;
	while (redir)
	{
		elem = (t_redir *)redir->content;
		status = dup_indir_elem(elem, &fd_in, &fd_out);
		if (status)
		{
			if (fd_in > -1)
				close(fd_in);
			if (fd_out > -1)
				close(fd_out);
			return (status);
		}
		redir = redir->next;
	}
	return (0);
}
