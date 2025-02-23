#include "minishell.h"


bool	is_syntax_error(t_token *split_token, t_token *start, t_token *end)
{
	return (split_token == start || split_token == end || 
		split_token->next == NULL || split_token->next->type != TOKEN_WORD ||
		split_token->prev == NULL || split_token->prev->type != TOKEN_WORD);
}

t_ast		*create_ast(t_token *start, t_token *end, t_mnsh *mnsh)
{
	t_ast	*node;
	t_token		*split_token;

	if (start == end->next)
		return (NULL);
	split_token = set_split_token(start, end);
	if (split_token == NULL)
	{
		return (create_ast_cmdnode(start, end, mnsh));
	}
	else if (is_syntax_error(split_token, start, end))
	{
		mnsh->last_exit_status = 2;
		printf("minishell: syntax error near unexpected token `%s'\n", split_token->word);
		return (NULL);
	}
	node = create_ast_opnode(split_token->word);
	if (!node)
		return (NULL);
	node->left_node = create_ast(start, split_token->prev, mnsh);
	node->right_node = create_ast(split_token->next, end, mnsh);
	return (node);
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

// ====================================

t_ast		*create_ast_opnode(char *op)
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

// ====================================

t_ast   	*create_ast_cmdnode(t_token	*start, t_token *end, t_mnsh *mnsh)
{
    t_ast   *node;
	t_token	*iterator;
	int		i;

	node = init_cmd_node();
	if (handle_indir(&node, start, end) == -2)
	{
		mnsh->last_exit_status = 2;
		return (NULL);
	}
	node->args = ft_calloc(cmd_args_count(start, end) + 1, sizeof(char *));
	if (!node->args)
		return (NULL);
	iterator = start;
	i = 0;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
			iterator = iterator->next;
		else
			node->args[i++] = ft_strdup(iterator->word);
		iterator = iterator->next;
	}
	return (node);
}

t_ast   	*init_cmd_node(void)
{
    t_ast   *node;

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


// ==========================

int			handle_indir(t_ast **node, t_token *start, t_token *end)
{
	t_token	*iterator;
	// int		status;

	iterator = start;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
		{
			if (check_indir_error(&iterator, &end) < 0)
			{
				free(*node);
				return (-2);
			}
			set_indir(node, &iterator);
			iterator = iterator->next;
		}
		iterator = iterator->next;
	}
	return (0);
}

int			check_indir_error(t_token **iterator, t_token **end)
{
	if (*iterator == *end || (*iterator)->next == NULL)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-2);
	}
	else if ((*iterator)->next->type == TOKEN_INDIR)
	{
		printf("minishell: syntax error near unexpected token `%s'\n", (*iterator)->word);
		return (-2);
	}
	else if (*(*iterator)->next->word == '\0')
	{
		printf("minishell: : No such file or directory\n");
		return (-2);
	}
	return (0);
}

void		set_indir(t_ast **node, t_token **iterator)
{
	if (!ft_strcmp((*iterator)->word, "<>"))
	{
		set_node_infile(iterator, node);
		set_node_outfile(iterator, node, OUT_TRUNC);
	}
	else if (!ft_strcmp((*iterator)->word, "<<"))
		set_node_heredoc(iterator, node);
	else if (!ft_strcmp((*iterator)->word, "<"))
		set_node_infile(iterator, node);
	else if (!ft_strcmp((*iterator)->word, ">>"))
		set_node_outfile(iterator, node, OUT_APPEND);
	else if (!ft_strcmp((*iterator)->word, ">"))
		set_node_outfile(iterator, node, OUT_TRUNC);
}

void    	set_node_infile(t_token **iterator, t_ast **node)
{
	t_list	*infile;

	infile = ft_lstnew((*iterator)->next->word);
	if (!infile)
		return ;
	ft_lstadd_back(&(*node)->infiles, infile);
	(*node)->instyle = IN_FILE;
}

void    set_node_heredoc(t_token **iterator, t_ast **node)
{
    t_list  *heredoc;
    t_list  *lst_iter;

	(*iterator) = (*iterator)->next;
    ft_free_str(&(*node)->heredoc_end);
    ft_free_strarray(&(*node)->heredoc);
	(*node)->instyle = IN_HEREDOC;
    (*node)->heredoc_end = ft_strdup(((*iterator)->word));
    heredoc = ft_lstnew(readline("heredoc> "));
    lst_iter = heredoc;
    while (ft_strcmp(lst_iter->content, (*node)->heredoc_end) != 0)
    {
        lst_iter->next = ft_lstnew(readline("heredoc> "));
        lst_iter = lst_iter->next;
    }
    lst_to_arr(heredoc, node);
    ft_lstclear(&heredoc, &free);
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

void    set_node_outfile(t_token **iterator, t_ast **node, t_outstyle style)
{
	t_list		*outfile;
	t_outfile	*word;

	outfile = NULL;
	word = ft_calloc(1, sizeof(t_outfile *));
	if (!word)
		return ;
	word->file = (*iterator)->next->word;
	if (!word->file)
		return ;
	word->outstyle = style;
	outfile = ft_lstnew(word);
	if (!outfile)
		return ;
	ft_lstadd_back(&(*node)->outfiles, outfile);
}


// =============================

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

