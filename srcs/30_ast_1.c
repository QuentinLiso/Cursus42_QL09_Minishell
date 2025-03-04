/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   30_ast_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:57:03 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 12:17:39 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ast_opnode(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	int		status;
	t_token	*split_token;

	split_token = set_split_token(start, end);
	status = create_ast_opnode(node, split_token->word);
	if (status)
		return (status);
	status = create_ast(&(*node)->left_node, start, split_token->prev, mnsh);
	if (status)
		return (status);
	status = create_ast(&(*node)->right_node, split_token->next, end, mnsh);
	if (status)
		return (status);
	return (0);
}

int	create_ast_opnode(t_ast **node, char *op)
{
	(*node) = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (ENOMEM);
	(*node)->node_type = NODE_OP;
	(*node)->op_type = set_op_type(op);
	(*node)->args = ft_calloc(2, sizeof(char *));
	if (!(*node)->args)
		return (ENOMEM);
	(*node)->args[0] = ft_strdup(op);
	if (!(*node)->args[0])
		return (ENOMEM);
	(*node)->args[1] = NULL;
	(*node)->redir = NULL;
	(*node)->left_node = NULL;
	(*node)->right_node = NULL;
	return (0);
}

t_optype	set_op_type(char *op)
{
	if (!op)
		return (OP_NULL);
	else if (!ft_strcmp(op, "||"))
		return (OP_OR);
	else if (!ft_strcmp(op, "&&"))
		return (OP_AND);
	else if (!ft_strcmp(op, "|"))
		return (OP_PIPE);
	else
		return (OP_NULL);
}

int	ast_cmdnode(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	int	status;

	status = create_cmd_node(node);
	if (status)
		return (status);
	status = handle_indir(node, start, end, mnsh);
	if (status)
		return (status);
	status = set_cmdnode_args(node, start, end);
	if (status)
		return (status);
	return (0);
}

int	create_cmd_node(t_ast **node)
{
	(*node) = ft_calloc(1, sizeof(t_ast));
	if ((*node) == NULL)
		return (ENOMEM);
	(*node)->node_type = NODE_CMD;
	(*node)->op_type = OP_NULL;
	(*node)->args = NULL;
	(*node)->redir = NULL;
	(*node)->left_node = NULL;
	(*node)->right_node = NULL;
	return (0);
}
