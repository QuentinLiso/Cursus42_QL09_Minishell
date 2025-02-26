#include "minishell.h"

// int	ast_mnsh(t_mnsh *mnsh, t_ast **node, t_token *start, t_token *end)
// {
// 	int	status;
// 	status = create_ast(mnsh, node, start, end);
// 	if (status)
// 	{
// 		ft_free_ast(mnsh->node);
// 		return (status);
// 	}
// 	return (0);
// }

int		create_ast(t_mnsh *mnsh, t_ast **node, t_token *start, t_token *end)
{
	t_token	*split_token;
	int		status;
	if (start == end->next)
		return (0);
	split_token = set_split_token(start, end);
	if (split_token == NULL)
	{
		*node = create_ast_cmdnode(start, end);
		if (!*node)
			return (ENOMEM);
		return (0);
	}
	else if (is_syntax_error(start, end, split_token))
		return (perror_mnsh(2, 4, "", "syntax error near unexpected token `",
			split_token->word, "'"));
	*node = create_ast_opnode(split_token->word);
	if (!*node)
		return (ENOMEM);
	status = create_ast(mnsh, &(*node)->left_node, start, split_token->prev);
	if (status)
		return (status);
	status = create_ast(mnsh, &(*node)->right_node, split_token->next, end);
	if (status)
		return (status);
	return (0);
}

t_token		*set_split_token(t_token *start, t_token *end)
{
	t_token	*iterator;
	t_token	*split_token;
	int	precedence;
	int	lowest_precedence;

	split_token = NULL;
	lowest_precedence = INT_MAX;
	iterator = start;
	while (iterator != end->next)
	{
		precedence = get_operator_precedence(iterator->word);
		if (precedence > 0 && precedence <= lowest_precedence)
		{
			split_token = iterator;
			lowest_precedence = precedence;
		}
		iterator = iterator->next;
	}
	return (split_token);
}

int			get_operator_precedence(char *op)
{
	if (!op)
		return (0);
	else if (!ft_strcmp(op, "||"))
		return (1);
	else if (!ft_strcmp(op, "&&"))
		return (2);
	else if (!ft_strcmp(op, "|"))
		return (3);
	return (0);
}

bool	is_syntax_error(t_token *start, t_token *end, t_token *split_token)
{
	return (split_token == start || split_token == end || 
		split_token->next == NULL || split_token->next->type != TOKEN_WORD ||
		split_token->prev == NULL || split_token->prev->type != TOKEN_WORD);
}

// ============================================

int create_ast_opnode(t_ast **node, char *op)
{
	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->node_type = NODE_OP;
	node->op_type = set_op_type(op);
	node->args = ft_calloc(2, sizeof(char *));
	if (!node->args)
	{
		free(node);
		return (NULL);
	}
	node->args[0] = ft_strdup(op);
	if (!node->args[0])
	{
		free(node);
		return (NULL);
	}
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

// ========================================

t_ast	*create_ast_cmdnode(t_token *start, t_token *end)
{
	t_ast	*node;

	node = init_cmd_node();
	if (!node)
		return (NULL);
	if (handle_indir(&node, start, end))
		return (NULL);
	if (set_cmdnode_args(&node, start, end))
		return (NULL);
	return (node);
}

t_ast	*init_cmd_node(void)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
    if (node == NULL)
		return (NULL);
	node->node_type = NODE_CMD;
	node->op_type = OP_NULL;
    node->args = NULL;
    node->infiles = NULL;
    node->outfiles = NULL;
	node->instyle = IN_NULL;
    node->heredoc = NULL;
    node->left_node = NULL;
    node->right_node = NULL;
    return (node);
}

// ==================================

int		handle_indir(t_ast **node, t_token *start, t_token *end)
{
	t_token	*iterator;
	int		status;

	iterator = start;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
		{
			status = is_indir_error(iterator, end);
			if (status)
				return (status);
			status = set_indir(node, iterator);
			if (status)
				return (status);
			iterator = iterator->next;
		}
		iterator = iterator->next;
	}
	return (0);
}

int		is_indir_error(t_token *iterator, t_token *end)
{
	if (iterator == end || !iterator->next)
		return (perror_mnsh(2, 1, "syntax error near unexpected token`\
				newline'"));
	else if (iterator->next->type == TOKEN_INDIR)
		return (perror_mnsh(2, 3, "syntax error near unexpected token`",
				iterator->word, "'"));
	else if (iterator->next->word && !*iterator->next->word)
		return (perror_mnsh(126, 2, "", "No such file or directory"));
	return (0);
}

int		set_indir(t_ast **node, t_token *iterator)
{
	int	status;

	status = 0;
	if (!ft_strcmp(iterator->word, "<>"))
	{
		if (set_node_infile(node, iterator))
			return (-12);
		if (set_node_outfile(node, iterator, OUT_TRUNC))
			return (-12);
	}
	else if (!ft_strcmp(iterator->word, "<<"))
		status = set_node_heredoc(node, iterator);
	else if (!ft_strcmp(iterator->word, "<"))
		status = set_node_infile(node, iterator);
	else if (!ft_strcmp(iterator->word, ">>"))
		status = set_node_outfile(node, iterator, OUT_APPEND);
	else if (!ft_strcmp(iterator->word, ">"))
		status = set_node_outfile(node, iterator, OUT_TRUNC);
	return (status);
}

int		set_node_infile(t_ast **node, t_token *iterator)
{
	t_list	*inlist_elem;

	inlist_elem = ft_lstnew(ft_strdup(iterator->next->word));
	if (!inlist_elem)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set node infile"));
	ft_lstadd_back(&(*node)->infiles, inlist_elem);
	(*node)->instyle = IN_FILE;
	return (0);
}

int		set_node_heredoc(t_ast **node, t_token *iterator)
{
    t_list  *heredoc;
    t_list  *lst_iter;

	iterator = iterator->next;
    ft_free_str(&(*node)->heredoc_end);
    ft_free_strarray(&(*node)->heredoc);
	(*node)->instyle = IN_HEREDOC;
    (*node)->heredoc_end = ft_strdup(iterator->word);
    heredoc = ft_lstnew(readline("heredoc> "));
    lst_iter = heredoc;
    while (ft_strcmp(lst_iter->content, (*node)->heredoc_end) != 0)
    {
        lst_iter->next = ft_lstnew(readline("heredoc> "));
        lst_iter = lst_iter->next;
    }
    lst_to_arr(node, heredoc);
    ft_lstclear(&heredoc, &free);
	return (0);
}

int		lst_to_arr(t_ast **node, t_list *heredoc)
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
	return (0);
}

int		set_node_outfile(t_ast **node, t_token *iterator, t_outstyle style)
{
	t_list		*outlist_elem;
	t_outfile	*outfile;

	outlist_elem = NULL;
	outfile = ft_calloc(1, sizeof(t_outfile *));
	if (!outfile)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set node outfile"));
	outfile->file = ft_strdup(iterator->next->word);
	if (!outfile->file)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set node outfile"));
	outfile->outstyle = style;
	outlist_elem = ft_lstnew(outfile);
	if (!outlist_elem)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set node outfile"));
	ft_lstadd_back(&(*node)->outfiles, outlist_elem);
	return (0);
}

// ====================================

int		set_cmdnode_args(t_ast **node, t_token *start, t_token *end)
{
	t_token	*iterator;
	int		i;

	(*node)->args = ft_calloc(cmd_args_count(start, end) + 1, sizeof(char *));
	if (!(*node)->args)
		return (perror_mnsh(ENOMEM, 1, "malloc err in set args"));
	iterator = start;
	i = 0;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
			iterator = iterator->next;
		else
		{
			(*node)->args[i] = ft_strdup(iterator->word);
			if (!(*node)->args[i++])
				return (perror_mnsh(ENOMEM, 1, "malloc err in set args"));
		}
		iterator = iterator->next;
	}
	return (0);
}

int     	cmd_args_count(t_token *start, t_token *end)
{
	t_token	*iterator;
    int		len;
	
	iterator = start;
	len = 0;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
		{
			iterator = iterator->next->next;
			continue;
		}
		iterator = iterator->next;
		len++;
	}
	return (len);
}

