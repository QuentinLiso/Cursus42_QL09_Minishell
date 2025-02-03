#include "minishell.h"

void	print_node(t_ast *node)
{
	ft_printf("====\nType : %d\n", node->node_type);
    printf("Cmd : %s\n", node->args[0]);
    print_infiles(node->infiles);
	print_strarray("Heredoc", node->heredoc);
	print_outfiles(node->outfiles);
	print_strarray("Args", node->args);
}

void	print_strarray(char *name, char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	ft_printf("%s : [", name);
	while (arr[++i])
		ft_printf("\"%s\", ", arr[i - 1]);
	ft_printf("\"%s\"]\n", arr[i - 1]);
}

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
		ft_printf("COMMAND : %s ", node->args[0]);
		if (node->args)
		{
			ft_printf("[");
			while (node->args[++i])
				ft_printf("%s ", node->args[i]);
			ft_printf("]");
		}
		ft_printf("\n");
		print_infiles(node->infiles);
		printf("Instyle : %d\n", node->instyle);
		print_outfiles(node->outfiles);
		print_strarray("Heredoc", node->heredoc);
	}
	print_ast(node->left_node, depth + 1);
	print_ast(node->right_node, depth + 1);
}

void	print_infiles(t_infiles *infiles)
{
	t_infiles *iter;

	iter = infiles;
	printf("Infiles : ");
	while (iter)
	{
		printf("%s ", iter->infile);
		iter = iter->next;
	}
	printf("\n");
}

void	print_outfiles(t_outfiles *outfiles)
{
	t_outfiles *iter;

	iter = outfiles;
	printf("Outfiles : ");
	while (iter)
	{
		printf("%s[%d] ", iter->outfile, iter->outstyle);
		iter = iter->next;
	}
	printf("\n");
}