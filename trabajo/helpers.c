#include "minishell.h"

void	print_node(t_ast *node)
{
	ft_printf("====\nType : %d\n", node->node_type);
	ft_printf("Value : %s\n", node->value);
	ft_printf("Args : \n");
	print_strarray(node->args);
}

void	print_ast(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = -1;
	while (++i < depth)
		ft_printf("\t");
	i = -1;
	if (node->node_type == NODE_OP)
		ft_printf("OPERATOR : %s (type : %d)\n", node->value, node->op_type);
	else
	{
		ft_printf("COMMAND : %s ", node->value);
		if (node->args)
		{
			ft_printf("[");
			while (node->args[++i])
				ft_printf("%s ", node->args[i]);
			ft_printf("]");
		}
		ft_printf("\n");
	}
	print_ast(node->left_node, depth + 1);
	print_ast(node->right_node, depth + 1);
}