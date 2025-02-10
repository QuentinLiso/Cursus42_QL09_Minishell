#include "minishell.h"

void	execute_ast(t_ast **node, t_mnsh *mnsh)
{
	if (!(*node))
		return ;
	if ((*node)->node_type == NODE_CMD)
		exec_ast_cmd(node, mnsh);
	else if ((*node)->node_type == NODE_OP)
		exec_ast_op(node, (*node)->op_type, mnsh);
}

void	exec_ast_cmd(t_ast **node, t_mnsh *mnsh)
{
	int		fd_inout[2];
	int		default_inout[2];

	default_inout[0] = dup(STDIN_FILENO);
	default_inout[1] = dup(STDOUT_FILENO);
	exec_ast_cmd_in(node, &fd_inout[0]);
	exec_ast_cmd_out(node, &fd_inout[1]);
	// expand_env_vars(&(*node)->args, mnsh);
	if (is_builtin((*node)->args[0]))
		mnsh->last_exit_status = exec_ast_cmd_builtin((*node)->args, mnsh);
	else
		mnsh->last_exit_status = exec_ast_cmd_external((*node)->args, mnsh);
	dup2(default_inout[0], STDIN_FILENO);
	dup2(default_inout[1], STDOUT_FILENO);
	close(default_inout[0]);
	close(default_inout[1]);
}

int	exec_ast_cmd_builtin(char **args, t_mnsh *mnsh)
{
	b_in(args[0], args, mnsh, &mnsh->env_mnsh);
	mnsh->last_exit_status = 0;
	(void)args;(void)mnsh;
	return (0);
}

int	exec_ast_cmd_external(char **args, t_mnsh *mnsh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return (1);
	}
	else if (pid == 0)
	{
		set_cmd_path(&args[0], mnsh->paths);
		execve(args[0], args, mnsh->env_mnsh);
		perror("execve failed");
		exit (127);
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}

//==========================================================================

void	expand_env_vars(char ***args, t_mnsh *mnsh)
{
	int	i;
	char	**tokens;
	// char	*buf;

	tokens = ft_calloc(128, sizeof(char *));

	i = -1;
	while((*args)[++i])
	{
		if ((*args)[i][0] == '\'')
		{
			// buf = &(*args)[i][1];
			// ft_free_str(&(*args)[i]);
			// (*args)[i] = buf;
			return ;
		}
		if (strtok_arg_env((*args)[i], &tokens, mnsh))
			return ;
		free((*args)[i]);
		(*args)[i] = ft_strcat_arr(tokens);
	}
	ft_free_strarray(&tokens);
}

t_error strtok_arg_env(char *s, char ***tokens, t_mnsh *mnsh)
{
    int     i;
    t_error tok_status;

    i = 0;
    while (*s)
    {
        tok_status = env_check(&s, tokens, &i, mnsh);
        if (tok_status == CONTINUE)
            continue;
        else if (tok_status > 0)
            return (tok_status);
    }
    (*tokens)[i] = NULL;
    return (ERR_NOERR);
}

t_error env_check(char **s, char ***tokens, int *i, t_mnsh *mnsh)
{
    t_error status;

    status = env_check_dollar(s, tokens, i, mnsh);
    if (status)
        return (status);
    status = env_check_regular(s, tokens, i);
    if (status)
        return (status);
    return (status);
}

t_error env_check_dollar(char **s, char ***tokens, int *i, t_mnsh *mnsh)
{
    char    *start;

    if (**s == '$')
    {
        start = *s;
        (*s)++;
        if (**s == '?')
        {
            (*s)++;
            (*tokens)[*i] = ft_itoa(mnsh->last_exit_status);
            if ((*tokens)[*i] == NULL)
                return (perror_malloc("env_check_dollar"));
            (*i)++;
            return (CONTINUE);
        }
        while (**s && (ft_isalnum(**s) || **s == '_'))
            (*s)++;
        (*tokens)[*i] = expand_env_var(start, *s, mnsh);
        if ((*tokens)[*i] == NULL)
            return (perror_malloc("env_check_dollar"));
        (*i)++;
        return (CONTINUE);
    }
    return (ERR_NOERR);
}

char    *expand_env_var(char *start, char *s, t_mnsh *mnsh)
{
    char    *env;
    char    *dup_buffer;
	
    dup_buffer = ft_strndup(start + 1, s - start - 1);
	env = ft_get_env_var(mnsh->env_mnsh, dup_buffer);
	free(dup_buffer);
	if (!env)
		return (NULL);
    return (env);
}

t_error env_check_regular(char **s, char ***tokens, int *i)
{
    char    *start;

    start = *s;
    while (**s && **s != '$')
        (*s)++;
    (*tokens)[*i] = ft_strndup(start, *s - start);
    if ((*tokens)[*i] == NULL)
        return (perror_malloc("env_check_regular"));
    (*i)++;
    return (CONTINUE);
}

char    *ft_strcat_arr(char **arr)
{
    size_t  len;
    int     iter[3];
    char    *d;

    len = 0;
    iter[0] = -1;
    while (arr[++iter[0]])
        len += ft_strlen(arr[iter[0]]);
    d = ft_calloc(len + 1, sizeof(char));
    if (!d)
        return (NULL);
    iter[0] = -1;
    iter[1] = 0;
    while (arr[++iter[0]])
    {
        iter[2] = 0;
        while (arr[iter[0]][iter[2]])
            d[iter[1]++] = arr[iter[0]][iter[2]++];
    }
    d[iter[1]] = 0;
    return (d);
}

//=========================================================================

void	set_cmd_path(char **cmd, char **paths)
{
	int		i;
	char 	*buf;
	struct stat	st;

	i = -1;
	while (paths[++i])
	{
		buf = ft_strjoin(paths[i], *cmd);
		if (stat(buf, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(*cmd);
			*cmd = buf;
			return ;	
		}
		free(buf);
	}
}

void	exec_ast_cmd_out(t_ast **node, int *fd)
{
	t_outfiles	*iterator;

	iterator = (*node)->outfiles;
	if (!iterator || !iterator->outfile)
		return ;
	while (iterator != NULL)
	{
		if (iterator->outstyle == OUT_TRUNC)
			exec_ast_cmd_outfile(iterator->outfile, fd, O_TRUNC);
		else if (iterator->outstyle == OUT_APPEND)
			exec_ast_cmd_outfile(iterator->outfile, fd, O_APPEND);
		iterator = iterator->next;
	}
}

void	exec_ast_cmd_outfile(char *outfile, int *fd, int flag)
{
	*fd = open(outfile, O_WRONLY | O_CREAT | flag, 0644);
	if (*fd < 0)
	{
		perror("open failed");
		exit (errno);
	}
	dup2(*fd, STDOUT_FILENO);
	close(*fd);
}

void	exec_ast_cmd_in(t_ast **node, int *fd)
{
	char	*last_infile;

	last_infile = check_infiles(node);
	if ((*node)->instyle == IN_FILE)
	{
		exec_ast_cmd_infile(last_infile, fd);
	}
	else if ((*node)->instyle == IN_HEREDOC)
	{
		exec_ast_cmd_heredoc(node);
	}
}

char	*check_infiles(t_ast **node)
{
	t_infiles	*iterator;
	int			check_access;

	iterator = (*node)->infiles;
	if (!iterator || !iterator->infile)
	{
		return (NULL);
	}
	while (iterator->infile)
	{
		check_access = access(iterator->infile, O_RDONLY);
		if (check_access < 0)
		{
			perror("access rdonly failed");
			exit (errno);
		}
		if (iterator->next == NULL)
		{
			return (iterator->infile);
		}
		iterator = iterator->next;
	}
	return (iterator->infile);
}

void	exec_ast_cmd_infile(char *last_infile, int *fd)
{
	*fd = open(last_infile, O_RDONLY);
	if (*fd < 0)
	{
		perror("open failed");
		exit (errno);
	}
	dup2(*fd, STDIN_FILENO);
	close(*fd);
}

void	exec_ast_cmd_heredoc(t_ast **node)
{
	char	**heredoc;
	int		fd[2];
	int		i;
	
	pipe(fd);
	heredoc = (*node)->heredoc;
	i = -1;

	while (heredoc[++i])
		ft_putendl_fd(heredoc[i], fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}

void	exec_ast_op(t_ast **node, t_optype op, t_mnsh *mnsh)
{
	if (op == OP_AND)
		exec_ast_op_and(node, mnsh);
	else if (op == OP_OR)
		exec_ast_op_or(node, mnsh);
	else if (op == OP_PIPE)
		exec_ast_op_pipe(node, mnsh);
}

void	exec_ast_op_and(t_ast **node, t_mnsh *mnsh)
{
	execute_ast(&((*node)->left_node), mnsh);
	if (mnsh->last_exit_status == 0)
		execute_ast(&(*node)->right_node, mnsh);
}

void	exec_ast_op_or(t_ast **node, t_mnsh *mnsh)
{
	execute_ast(&((*node)->left_node), mnsh);
	if (mnsh->last_exit_status != 0)
		execute_ast(&(*node)->right_node, mnsh);
}

void	exec_ast_op_pipe(t_ast **node, t_mnsh *mnsh)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		fd[2];

	pipe(fd);
	left_pid = fork();
	if (left_pid == 0)
		left_pipe(node, &fd, mnsh);
	right_pid = fork();
	if (right_pid == 0)
		right_pipe(node, &fd, mnsh);
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}

void	left_pipe(t_ast **node, int (*fd)[2], t_mnsh *mnsh)
{
	close((*fd)[0]);
	dup2((*fd)[1], STDOUT_FILENO);
	close((*fd)[1]);
	execute_ast(&(*node)->left_node, mnsh);
	exit(0);
}

void	right_pipe(t_ast **node, int (*fd)[2], t_mnsh *mnsh)
{
	close((*fd)[1]);
	dup2((*fd)[0], STDIN_FILENO);
	close((*fd)[0]);
	execute_ast(&(*node)->right_node, mnsh);
	exit(0);
}
