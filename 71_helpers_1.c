/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   71_helppers_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:20:05 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 17:20:47 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ast(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = -1;
	while (++i < depth)
		ft_printf("\t");
	i = 0;
	if (node->node_type == NODE_OP)
		ft_printf("OPERATOR : %s (type : %d)\n", node->args[0], node->op_type);
	else
	{
		ft_printf("COMMAND : ");
		if (node->args)
		{
			ft_printf("%s ", node->args[0]);
			ft_printf("[");
			while (node->args[++i])
				ft_printf("%s ", node->args[i]);
			ft_printf("]");
		}
		ft_printf("\n");
		print_redir(node->redir);
	}
	print_ast(node->left_node, depth + 1);
	print_ast(node->right_node, depth + 1);
}

void	print_redir(t_list *redir_files)
{
	t_list 		*iter;
	t_redir		*redir_file;

	iter = redir_files;
	printf("Redir files : ");
	while (iter)
	{
		redir_file = (t_redir *)iter->content;
		printf("%s[%d] ", redir_file->file, redir_file->style);
		iter = iter->next;
	}
	printf("\n");
}

void	print_tokis(t_token *tokis)
{
	t_token *iter;

	iter = tokis;
	printf("Tokis : \n[");
	while (iter)
	{
		if (!iter->word)
			printf("(null)[%d]", iter->type);
		else
			printf("%s[%d]", iter->word, iter->type);			
		printf(", ");
		iter = iter->next;
	}
	printf("]\n");
}

void	check_open_fds(void)
{
	int	fd;
	int	bytes;

	fd = 0;
	while (fd < 10)
	{
		if (ioctl(fd, FIONREAD, &bytes) != -1)
			printf("FD is open : %d\n", fd);
		fd++;
	}
}