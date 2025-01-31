#include "minishell.h"

t_ast	*create_ast(char **tokens, int start, int end)
{
	t_ast	*node;
	int		split_index;
	int		args_count;

	if (start > end)
		return (NULL);
	split_index = set_split_index(tokens, start, end);
	if (split_index == -1)
	{
		args_count = set_args_count(tokens, start, end);
		return (create_ast_cmdnode(tokens[start], tokens + start + 1, args_count));
	}
	node = create_ast_opnode(tokens[split_index]);
	node->left_node = create_ast(tokens, start, split_index - 1);
	node->right_node = create_ast(tokens, split_index + 1, end);
	return (node);
}

int	set_split_index(char **tokens, int start, int end)
{
	int	i;
	int	split_index;
	int	precedence;
	int	lowest_precedence;


	split_index = -1;
	lowest_precedence = INT_MAX;
	i = start - 1;
	while (++i <= end)
	{
		precedence = get_operator_precedence(tokens[i]);
		if (precedence > 0 && precedence <= lowest_precedence)
		{
			split_index = i;
			lowest_precedence = precedence;
		}
	}
	return (split_index);
}

int	set_args_count(char **tokens, int start, int end)
{
	int	i;

	i = start - 1;
	while (++i <= end)
		if (get_operator_precedence(tokens[i]) > 0)
			return (i - start - 1);
	return (i - start - 1);
}

t_ast	*create_ast_opnode(char *op)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_OP;
	node->value = ft_strdup(op);
	node->args = NULL;
	node->left_node = NULL;
	node->right_node = NULL;
	return (node);
}

t_ast	*create_ast_cmdnode(char *cmd, char **args, int arg_count)
{
	t_ast	*node;
	int		i;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->value = ft_strdup(cmd);
	if (!node->value)
		return (NULL);
	node->args = ft_calloc(arg_count + 1, sizeof(char *));
	if (!node->args)
		return (NULL);
	i = -1;
	while (++i < arg_count)
	{
		node->args[i] = ft_strdup(args[i]);
		if (!node->args[i])
			return (NULL);
	}
	node->args[arg_count] = NULL;
	node->left_node = NULL;
	node->right_node = NULL;
	return (node);
}

int	get_operator_precedence(char *op)
{
	if (!ft_strcmp(op, "||"))
		return (1);
	else if (!ft_strcmp(op, "&&"))
		return (2);
	else if (!ft_strcmp(op, "|"))
		return (3);
	else if (!ft_strcmp(op, "<") || !ft_strcmp(op, ">") ||
		!ft_strcmp(op, "<<") || !ft_strcmp(op, ">>"))
		return (4);
	return (0);
}


void	free_ast(t_ast *root_node)
{
	int	i;

	if (!root_node)
		return ;
	free_ast(root_node->left_node);
	free_ast(root_node->right_node);
	free(root_node->value);
	i = -1;
	if (root_node->type == NODE_CMD && root_node->args)
	{
		while (root_node->args[++i])
			free(root_node->args[i]);
		free(root_node->args);
	}
	free(root_node);
}