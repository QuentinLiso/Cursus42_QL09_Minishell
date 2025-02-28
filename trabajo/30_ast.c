#include "minishell.h"

int		ast_mnsh(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	int	status;

	if (!start || !end)
		return (0);
	status = create_ast(node, start, end, mnsh);
	if (status)
	{
		mnsh->last_exit_status = status;
		return (status);
	}
	return (0);
}

int		create_ast(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	t_token	*split_token;

	if (start == end->next)
		return (0);
	split_token = set_split_token(start, end);
	if (split_token == NULL)
		return (ast_cmdnode(node, start, end, mnsh));
	else if (is_syntax_error(start, end, split_token))
		return (errno_to_exit(2));
	return (ast_opnode(node, start, split_token, end, mnsh));
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

bool		is_syntax_error(t_token *start, t_token *end, t_token *split_token)
{
	if	(split_token == start || split_token == end || 
		split_token->next == NULL || split_token->next->type == TOKEN_OPERATOR ||
		split_token->prev == NULL || split_token->prev->type == TOKEN_OPERATOR)
	{
		perror2_mnsh(2, 4, ": ", "syntax error near unexpected token `",
			split_token->word, "'");
		return (true);
	}
	else
	{
		return (false);
	}
}

// ============================================

int		ast_opnode(t_ast **node, t_token *start, t_token *split_tok, t_token *end, t_mnsh *mnsh)
{
	int		status;

	status = create_ast_opnode(node, split_tok->word);
	if (status)
		return (status);
	status = create_ast(&(*node)->left_node, start, split_tok->prev, mnsh);
	if (status)
		return (status);
	status = create_ast(&(*node)->right_node, split_tok->next, end, mnsh);
	if (status)
		return (status);
	return (0);
}

int 	create_ast_opnode(t_ast **node, char *op)
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

// ========================================

int		ast_cmdnode(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	int		status;

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

int		create_cmd_node(t_ast **node)
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

// ==================================

int		handle_indir(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
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
			status = set_indir(node, iterator, mnsh);
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
		return (perror2_mnsh(2, 3, ": ", "syntax error near unexpected token",
			" `newline'"));
	else if (iterator->next->type == TOKEN_INDIR)
		return (perror2_mnsh(2, 5, ": ", "syntax error near unexpected token",
			" `", iterator->word, "'"));
	else if (iterator->next->word && !*iterator->next->word)
		return (perror_mnsh(1, 2, "", "No such file or directory"));
	return (0);
}

int		set_indir(t_ast **node, t_token *iterator, t_mnsh *mnsh)
{
	if (!ft_strcmp(iterator->word, "<>"))
	{
		if (set_node_redir(node, iterator, REDIR_IN))
			return (ENOMEM);
		if (set_node_redir(node, iterator, REDIR_OUT))
			return (ENOMEM);
	}
	else if (!ft_strcmp(iterator->word, "<<"))
		return (set_node_heredoc(node, iterator, mnsh));
	else if (!ft_strcmp(iterator->word, "<"))
		return (set_node_redir(node, iterator, REDIR_IN));
	else if (!ft_strcmp(iterator->word, ">>"))
		return (set_node_redir(node, iterator, REDIR_APPEND));
	else if (!ft_strcmp(iterator->word, ">"))
		return (set_node_redir(node, iterator, REDIR_OUT));
	return (0);
}


int		set_node_redir(t_ast **node, t_token *iterator, t_redirstyle style)
{
	t_list		*redir_elem;
	t_redir		*redir_file;

	redir_elem = NULL;
	redir_file = ft_calloc(1, sizeof(t_redir));
	if (!redir_file)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set redir"));
	redir_file->file = ft_strdup(iterator->next->word);
	if (!redir_file->file)
	{
		free(redir_file);
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set redir"));
	}
	redir_file->style = style;
	redir_file->heredoc = NULL;
	redir_elem = ft_lstnew(redir_file);
	if (!redir_elem)
	{
		free_redir(redir_file);
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set redir"));
	}
	ft_lstadd_back(&(*node)->redir, redir_elem);
	return (0);
}

int		set_node_heredoc(t_ast **node, t_token *iterator, t_mnsh *mnsh)
{
	t_list	*redir_elem;
	t_redir	*redir_file;

	redir_elem = NULL;
	redir_file = ft_calloc(1, sizeof(t_redir));
	if (!redir_file)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set heredoc"));
	redir_file->file = set_heredoc_name();
	if (!redir_file->file)
	{
		free(redir_file);
		return (perror_mnsh(ENOMEM, 1, "malloc err in set heredoc"));
	}
	redir_file->style = REDIR_HEREDOC;
	redir_elem = ft_lstnew(redir_file);
	if (!redir_elem)
	{
		free_redir(redir_file);
		return (perror_mnsh(ENOMEM, 1, "malloc err in set heredoc"));
	}
	ft_lstadd_back(&(*node)->redir, redir_elem);
	return (create_heredoc(redir_file->file, iterator->next->word, mnsh));
}

char	*set_heredoc_name()
{
	static int count = 1;
	char	*count_itoa;
	char	*heredoc_name;

	if (count < 0)
		count = 1;
	count_itoa = ft_itoa(count);
		(count)++;
	if (count < 0)
		count = 1;
	if (!count_itoa)
		return (NULL);
	heredoc_name = ft_strjoin("mnsh-hdoc-tmp-", count_itoa);
	free_str(&count_itoa);
	if (!heredoc_name)
		return (NULL);
	return (heredoc_name);
}

int		create_heredoc(char *heredoc, char *heredoc_end, t_mnsh *mnsh)
{
	int		fd;

	fd = open(heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	fill_heredoc(fd, heredoc_end, mnsh);
	close (fd);
	return (0);
}

void	fill_heredoc(int fd, char *heredoc_end, t_mnsh *mnsh)
{
	char	*prompt;
	int		count;

	count = 0;
	while (1)
	{
		prompt = readline("heredoc> ");
		if (!prompt)
		{
			warn_heredoc(mnsh->line_count, heredoc_end);
			break;
		}
		count++;
		if (!ft_strcmp(prompt, heredoc_end))
			break ;
		ft_putendl_fd(prompt, fd);
		free_str(&prompt);
	}
	free_str(&prompt);
	mnsh->line_count += count;
}

void	warn_heredoc(int line, char *heredoc_end)
{
	ft_putstr_fd("minishell: warning: here-document at line ", STDERR_FILENO);
	ft_putnbr_fd(line, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(heredoc_end, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
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
