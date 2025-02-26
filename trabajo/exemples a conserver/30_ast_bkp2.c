#include "minishell.h"

int		create_ast_mnsh(t_mnsh *mnsh, t_ast **node, t_token *start, t_token *end)
{
	t_token		*split_token;

	if (start == end->next)
		return (0);
	split_token = set_split_token(start, end);
	if (split_token == NULL)
	{
		*node = ast_cmd_node(mnsh, start, end);
	}
	else if (is_syntax_error(split_token, start, end))
		return (free_ast_ret(mnsh->node, -2));
	
	return (ast_op_node(mnsh, node, start, split_token, end));
}

// ======================================

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

// ======================================

int	ast_op_node(t_mnsh *mnsh, t_ast **node, t_token *start, t_token *split_token, t_token *end)
{
	int	status;

	status = init_ast_op_node(node, split_token->word);
	if (status)
		return (free_ast_ret(mnsh->node, status));

	status = create_ast_mnsh(mnsh, &(*node)->left_node, start, split_token->prev);
	if (status)
		return (free_ast_ret(mnsh->node, status));
	status = create_ast_mnsh(mnsh, &(*node)->right_node, split_token->next, end);
	if (status)
		return (free_ast_ret(mnsh->node, status));
	return (0);
}

bool	is_syntax_error(t_token *split_token, t_token *start, t_token *end)
{
	return (split_token == start || split_token == end || 
		split_token->next == NULL || split_token->next->type != TOKEN_WORD ||
		split_token->prev == NULL || split_token->prev->type != TOKEN_WORD);
}

int		init_ast_op_node(t_ast **node, char *op)
{
	(*node) = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (-12);
	(*node)->node_type = NODE_OP;
	(*node)->op_type = set_op_type(op);
	(*node)->args = ft_calloc(2, sizeof(char *));
	if (!(*node)->args)
		return (-12);
	(*node)->args[0] = ft_strdup(op);
	(*node)->args[1] = NULL;
    (*node)->infiles = NULL;
    (*node)->outfiles = NULL;
    (*node)->heredoc = NULL;
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

// ====================================

t_ast	*ast_cmd_node(t_mnsh *mnsh, t_token *start, t_token *end)
{
	int		status;

	if (init_cmd_node(node) < 0)
		return (free_ast_ret(mnsh->node, -12));
	status = handle_indir(node, start, end);
	if (status)
		return (free_ast_ret(mnsh->node, status));
	status = set_cmd_args(node, start, end);
	if (status)
		return (free_ast_ret(mnsh->node, status));
	return (0);
}

int		init_cmd_node(t_ast **node)
{
    *node = ft_calloc(1, sizeof(t_ast));
    if (!(*node))
		return (perror_mnsh(-12, 1, "malloc err in function init_cmd_node"));
	(*node)->node_type = NODE_CMD;
	(*node)->op_type = OP_NULL;
    (*node)->args = NULL;
    (*node)->infiles = NULL;
    (*node)->outfiles = NULL;
	(*node)->instyle = IN_NULL;
    (*node)->heredoc = NULL;
    (*node)->left_node = NULL;
    (*node)->right_node = NULL;
    return (0);
}

int		set_cmd_args(t_ast **node, t_token *start, t_token *end)
{
	t_token	*iterator;
	int		i;

	(*node)->args = ft_calloc(cmd_args_count(start, end) + 1, sizeof(char *));
	if (!(*node)->args)
		return (perror_mnsh(-12, 1, "malloc err in function init_cmd_node"));
	iterator = start;
	i = 0;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
			iterator = iterator->next;
		else
			(*node)->args[i++] = ft_strdup(iterator->word);
		iterator = iterator->next;
	}
	return (0);
}

int     cmd_args_count(t_token *start, t_token *end)
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

// ==========================

int		handle_indir(t_ast **node, t_token *start, t_token *end)
{
	t_token	*iterator;
	int		status;

	iterator = start;
	status = 0;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
		{
			if (check_indir_error(&iterator, &end) < 0)
				return (-2);
			status = set_indir(node, &iterator);
			if (status < 0)
				return (status);
			iterator = iterator->next;
		}
		iterator = iterator->next;
	}
	return (status);
}

int		check_indir_error(t_token **iterator, t_token **end)
{
	if (*iterator == *end || (*iterator)->next == NULL)
		return (perror_mnsh(-2, 1, "syntax error near unexpected token`\
			newline'"));
	else if ((*iterator)->next->type == TOKEN_INDIR)
		return (perror_mnsh(-2, 1, "syntax error near unexpected token `",
			(*iterator)->word, "'"));
	else if (*(*iterator)->next->word == '\0')
		return (perror_mnsh(-2, 2, "", "No such file or directory"));
	return (0);
}

int		set_indir(t_ast **node, t_token **iterator)
{
	int	status;

	status = 0;
	if (!ft_strcmp((*iterator)->word, "<>"))
	{
		if (set_node_infile(iterator, node))
			return (-12);
		if (set_node_outfile(iterator, node, OUT_TRUNC))
			return (-12);
	}
	else if (!ft_strcmp((*iterator)->word, "<<"))
		status = set_node_heredoc(iterator, node);
	else if (!ft_strcmp((*iterator)->word, "<"))
		status = set_node_infile(iterator, node);
	else if (!ft_strcmp((*iterator)->word, ">>"))
		status = set_node_outfile(iterator, node, OUT_APPEND);
	else if (!ft_strcmp((*iterator)->word, ">"))
		status = set_node_outfile(iterator, node, OUT_TRUNC);
	return (status);
}

int    	set_node_infile(t_token **iterator, t_ast **node)
{
	t_list	*infile;

	infile = ft_lstnew(ft_strdup((*iterator)->next->word));
	if (!infile)
		return (perror_mnsh(-12, 1, "malloc err in func set node infile"));
	ft_lstadd_back(&(*node)->infiles, infile);
	(*node)->instyle = IN_FILE;
	return (0);
}

int    set_node_heredoc(t_token **iterator, t_ast **node)
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
	return (0);
}

int    lst_to_arr(t_list *heredoc, t_ast **node)
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



int    set_node_outfile(t_token **iterator, t_ast **node, t_outstyle style)
{
	t_list		*outfile;
	t_outfile	*word;

	outfile = NULL;
	word = ft_calloc(1, sizeof(t_outfile *));
	if (!word)
		return (perror_mnsh(-12, 1, "malloc err in func set node outfile"));
	word->file = ft_strdup((*iterator)->next->word);
	word->outstyle = style;
	outfile = ft_lstnew(word);
	if (!outfile)
		return (perror_mnsh(-12, 1, "malloc err in func set node outfile"));
	ft_lstadd_back(&(*node)->outfiles, outfile);
	return (0);
}
