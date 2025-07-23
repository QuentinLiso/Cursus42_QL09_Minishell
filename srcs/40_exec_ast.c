/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:06:42 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/05 13:48:32 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(t_ast **node, t_mnsh *mnsh)
{
	if (!(*node) || !(*node)->args || !(*node)->args[0])
	{
		mnsh->last_exit_status = 0;
		return (mnsh->last_exit_status);
	}
	if ((*node)->node_type == NODE_CMD)
	{
		if (set_mnsh_last_arg(node, mnsh))
		{
			mnsh->last_exit_status = errno_to_exit(ENOMEM);
			return (mnsh->last_exit_status);
		}
		mnsh->last_exit_status = exec_ast_cmd(node, mnsh);
		return (mnsh->last_exit_status);
	}
	else if ((*node)->node_type == NODE_OP)
	{
		mnsh->last_exit_status = exec_ast_op(node, (*node)->op_type, mnsh);
		return (mnsh->last_exit_status);
	}
	return (0);
}

int	set_mnsh_last_arg(t_ast **node, t_mnsh *mnsh)
{
	int		len;

	len = ft_strarrlen((*node)->args);
	if (len < 1)
		return (0);
	return (edit_env_var(&mnsh->env_mnsh_lst, "_", (*node)->args[len - 1]));
}

int	exec_ast_cmd(t_ast **node, t_mnsh *mnsh)
{
	int		default_inout[2];
	int		status;

	status = init_default_fd(&default_inout);
	if (status)
		return (status);
	status = exec_ast_cmd_indir((*node)->redir);
	if (status)
	{
		clean_default_fd(&default_inout);
		return (status);
	}
	if (is_builtin((*node)->args[0]))
		status = exec_ast_cmd_builtin((*node)->args, mnsh);
	else
		status = exec_ast_cmd_external((*node)->args, mnsh);
	if (status)
	{
		clean_default_fd(&default_inout);
		return (status);
	}
	return (clean_default_fd(&default_inout));
}

int	init_default_fd(int (*default_inout)[2])
{
	(*default_inout)[0] = dup(STDIN_FILENO);
	if ((*default_inout)[0] < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	(*default_inout)[1] = dup(STDOUT_FILENO);
	if ((*default_inout)[1] < 0)
	{
		close((*default_inout)[0]);
		return (perror_mnsh(1, 1, strerror(errno)));
	}
	return (0);
}

int	clean_default_fd(int (*default_inout)[2])
{
	int	status;

	status = 0;
	if (dup2((*default_inout)[0], STDIN_FILENO) < 0)
	{
		perror_mnsh(1, 1, strerror(errno));
		status = 1;
	}
	close((*default_inout)[0]);
	(*default_inout)[0] = -1;
	if (dup2((*default_inout)[1], STDOUT_FILENO) < 0)
	{
		perror_mnsh(1, 1, strerror(errno));
		status = 1;
	}
	close((*default_inout)[1]);
	(*default_inout)[1] = -1;
	return (status);
}
