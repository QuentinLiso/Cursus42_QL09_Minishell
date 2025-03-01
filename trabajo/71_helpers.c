#include "minishell.h"

void	print_node(t_ast *node)
{
	ft_printf("====\nType : %d\n", node->node_type);
    printf("Cmd : %s\n", node->args[0]);
    print_redir(node->redir);
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

void	print_strarray_raw(char **arr, char sep)
{
	int	i;

	i = -1;
	if (!arr || !arr[0])
		return ;
	while(arr[++i] != NULL)
	{
		ft_putstr_fd(arr[i], STDOUT_FILENO);
		if (arr[i + 1] != NULL)
			ft_putchar_fd(sep, STDOUT_FILENO);
	}
}

void	print_strarray_endl(char *name, char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	ft_printf("%s\n", name);
	while (arr[++i])
		ft_printf("\"%s\"\n", arr[i - 1]);
	ft_printf("\"%s\"\n", arr[i - 1]);
}

void	print_env(char **env)
{
	int	i;

	i = -1;
	if (!env)
		return ;
	while(env[++i])
		ft_printf("%s\n", env[i]);
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