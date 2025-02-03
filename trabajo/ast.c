#include "minishell.h"

t_ast	*create_ast(char **tokens, int start, int end)
{
	t_ast	*node;
	int		split_index;
	char	**buf;

	if (start > end)
		return (NULL);
	split_index = set_split_index(tokens, start, end);
	if (split_index == -1)
	{
		buf = &tokens[start];
		return (create_ast_cmdnode(&buf, end));
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

int	get_operator_precedence(char *op)
{
	if (!ft_strcmp(op, "||"))
		return (1);
	else if (!ft_strcmp(op, "&&"))
		return (2);
	else if (!ft_strcmp(op, "|"))
		return (3);
	return (0);
}

t_ast	*create_ast_opnode(char *op)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->node_type = NODE_OP;
	node->op_type = set_op_type(op);
	node->args = ft_calloc(2, sizeof(char *));
	if (!node->args)
		return (NULL);
	node->args[0] = ft_strdup(op);
	node->args[1] = NULL;
    node->infiles = NULL;
    node->outfiles = NULL;
    node->heredoc = NULL;
    node->left_node = NULL;
    node->right_node = NULL;
	return (node);
}

t_optype	set_op_type(char *op)
{
	if (!ft_strcmp(op, "||"))
		return (OP_OR);
	else if (!ft_strcmp(op, "&&"))
		return (OP_AND);
	else if (!ft_strcmp(op, "|"))
		return (OP_PIPE);
	else
		return (OP_NULL);
}

t_ast   *create_ast_cmdnode(char ***cmd_tok, int end)
{
    t_ast   *node;
    int     i;
    int     j;

    node = init_cmd_node(cmd_tok, end);
    i = 0;
    j = 1;
    while ((*cmd_tok)[i] && !is_operator((*cmd_tok)[i], AST_OPERATORS) && i <= end)
    {
        if (ft_strcmp((*cmd_tok)[i], ">>") == 0)
            set_node_outfile(cmd_tok, &node, &i, OUT_APPEND);
        else if (ft_strcmp((*cmd_tok)[i], ">") == 0)
            set_node_outfile(cmd_tok, &node, &i, OUT_TRUNC);
        else if (ft_strcmp((*cmd_tok)[i], "<<") == 0)
            set_node_heredoc(cmd_tok, &node, &i);
        else if (ft_strcmp((*cmd_tok)[i], "<") == 0)
            set_node_infile(cmd_tok, &node, &i);
        else if (node->args[0] == NULL)
            node->args[0] = ft_strdup((*cmd_tok)[i++]);
        else
            node->args[j++] = ft_strdup((*cmd_tok)[i++]);
    }
    return (node);
}

t_ast   *init_cmd_node(char ***cmd_tok, int end)
{
    t_ast   *node;

    node = ft_calloc(1, sizeof(t_ast));
    if (node == NULL)
		return (NULL);
	node->node_type = NODE_CMD;
	node->op_type = OP_NULL;
    node->args = ft_calloc(final_args_count((*cmd_tok), end) + 2, sizeof(char *));
    if (!node->args)
        return (NULL);
    node->infiles = NULL;
    node->outfiles = NULL;
	node->instyle = IN_NULL;
    node->heredoc = NULL;
    node->left_node = NULL;
    node->right_node = NULL;
    return (node);
}

int     final_args_count(char **cmd_tok, int end)
{
    int i;
    int len;

    i = 0;
    len = 0;
    while (cmd_tok[i] && !is_operator(cmd_tok[i], AST_OPERATORS) && i <= end)
    {
        if (is_indir(cmd_tok[i]))
        {
            i+=2;
            continue;
        }
        len++;
        i++;
    }
    return (len);
}

void    set_node_outfile(char ***tok, t_ast **node, int *i, t_outstyle style)
{
	t_outfiles	*outfile;

	outfile = outfiles_new((*tok)[*i + 1], style);
	if (!outfile)
		return ;
	outfiles_add_back(&(*node)->outfiles, outfile);
    (*i) += 2; 
}

void    set_node_infile(char ***tok, t_ast **node, int *i)
{
	t_infiles	*infile;

	infile = infiles_new((*tok)[*i + 1]);
	if (!infile)
		return;
	infiles_add_back(&(*node)->infiles, infile);
	(*node)->instyle = IN_FILE;
    (*i) += 2; 
}

void    set_node_heredoc(char ***cmd_tok, t_ast **node, int *i)
{
    t_list  *heredoc;
    t_list  *lst_iter;

    ft_free_str(&(*node)->heredoc_end);
    ft_free_strarray(&(*node)->heredoc);
	(*node)->instyle = IN_HEREDOC;
    (*node)->heredoc_end = ft_strdup((*cmd_tok)[*i + 1]);
    heredoc = ft_lstnew(readline("heredoc> "));
    lst_iter = heredoc;
    while (ft_strcmp(lst_iter->content, (*node)->heredoc_end) != 0)
    {
        lst_iter->next = ft_lstnew(readline("heredoc> "));
        lst_iter = lst_iter->next;
    }
    lst_to_arr(heredoc, node);
    ft_lstclear(&heredoc, &free);
    (*i) += 2;
}

void    lst_to_arr(t_list *heredoc, t_ast **node)
{
    int len;
    int i;

    len = ft_lstsize(heredoc);
    (*node)->heredoc = ft_calloc(len + 1, sizeof(char *));
    i = 0;
    while (heredoc->next)
    {
        (*node)->heredoc[i++] = ft_strdup((char *)heredoc->content);
        heredoc = heredoc->next;
    }
}

void	free_ast(t_ast *root_node)
{
	int	i;

	if (!root_node)
		return ;
	free_ast(root_node->left_node);
	free_ast(root_node->right_node);
	i = -1;
	if (root_node->node_type == NODE_CMD && root_node->args)
	{
		while (root_node->args[++i])
			free(root_node->args[i]);
		free(root_node->args);
	}
	free(root_node);
}