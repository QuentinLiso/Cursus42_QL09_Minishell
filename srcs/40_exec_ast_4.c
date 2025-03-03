/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:12:39 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 16:13:14 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int		get_cmd_path(char **execfile, char *cmd, char **paths)
{
	int		i;
	char 	*buf;
	struct stat	st;

	i = -1;
	while (paths[++i])
	{
		buf = ft_strjoin(paths[i], cmd);
		if (!buf)
			return (perror_mnsh(ENOMEM, 1, "err malloc get cmd path"));
		if (stat(buf, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			*execfile = buf;
			return (0);
		}
		else
			free_str(&buf);
	}
	return (ENOENT);
}

int		ft_execve(char **execfile, char **args, t_mnsh *mnsh)
{
	char	**env;

	env = env_lst_to_arr(mnsh->env_mnsh_lst);
	if (!env)
		return (perror_mnsh(12, 1, "err malloc env for exec"));
	execve(*execfile, args, env);
	free_strarray(&env);
	free_str(execfile);	
	if (errno == ENOENT)
		return (perror_mnsh(127, 2, args[0], strerror(ENOENT)));
	else if (errno == EACCES)
		return (perror_mnsh(126, 2, args[0], strerror(EACCES)));
	else
		return (perror_mnsh(1, 2, args[0], strerror(errno)));
}

int		exec_ast_op(t_ast **node, t_optype op, t_mnsh *mnsh)
{
	if (op == OP_AND)
		return (exec_ast_op_and(node, mnsh));
	else if (op == OP_OR)
		return (exec_ast_op_or(node, mnsh));
	else if (op == OP_PIPE)
		return (exec_ast_op_pipe(node, mnsh));
	else
		return (0);
}

int		exec_ast_op_and(t_ast **node, t_mnsh *mnsh)
{
	int	status;

	status = execute_ast(&((*node)->left_node), mnsh);
	if (mnsh->last_exit_status == 0)
		return (execute_ast(&(*node)->right_node, mnsh));
	return (status);
}

int		exec_ast_op_or(t_ast **node, t_mnsh *mnsh)
{
	int	status;

	status = execute_ast(&((*node)->left_node), mnsh);
	if (mnsh->last_exit_status != 0)
		return (execute_ast(&(*node)->right_node, mnsh));
	return (status);
}
