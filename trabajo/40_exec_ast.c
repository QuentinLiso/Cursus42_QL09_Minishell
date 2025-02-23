#include "minishell.h"

void	execute_ast(t_ast **node, t_mnsh *mnsh)
{
	if (!(*node) || !(*node)->args || !(*node)->args[0])
		return ;
	if ((*node)->node_type == NODE_CMD)
		exec_ast_cmd(node, mnsh);
	else if ((*node)->node_type == NODE_OP)
	{
		exec_ast_op(node, (*node)->op_type, mnsh);
	}	
}

void	exec_ast_cmd(t_ast **node, t_mnsh *mnsh)
{
	int		fd_inout[2];
	int		default_inout[2];

	default_inout[0] = dup(STDIN_FILENO);
	default_inout[1] = dup(STDOUT_FILENO);
	exec_ast_cmd_in(node, &fd_inout[0]);
	exec_ast_cmd_out(node, &fd_inout[1]);
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
		check_and_execute_cmd(args, mnsh);
		return (1);
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}

void	check_and_execute_cmd(char **args, t_mnsh *mnsh)
{
	struct stat	st;
	char		*exec_file;

	if (!args || !args[0])
		return ;
	if (*args[0] && !ft_strchr(args[0], '/'))
		exec_file = get_cmd_path(args[0], mnsh->paths);
	else
		exec_file = ft_strdup(args[0]);
	if (!exec_file || *exec_file == '\0')
		ft_perror_exit_mnsh(args[0], "command not found", 127, &exec_file);
	if (stat(exec_file, &st) == -1)
		ft_perror_exit_mnsh(args[0], strerror(ENOENT), 127, &exec_file);
	if (S_ISDIR(st.st_mode))
		ft_perror_exit_mnsh(args[0], strerror(EISDIR), 126, &exec_file);
	if (!(S_IXUSR & st.st_mode))
		ft_perror_exit_mnsh(args[0], strerror(EACCES), 126, &exec_file);
	execve(exec_file, args, mnsh->env_mnsh);
	if (errno == ENOENT)
		ft_perror_exit_mnsh(args[0], strerror(ENOENT), 127, &exec_file);
	else if (errno == EACCES)
		ft_perror_exit_mnsh(args[0], strerror(EACCES), 126, &exec_file);
	else
		ft_perror_exit_mnsh(args[0], strerror(errno), 1, &exec_file);
}

char	*get_cmd_path(char *cmd, char **paths)
{
	int		i;
	char 	*buf;
	struct stat	st;

	i = -1;
	while (paths[++i])
	{
		buf = ft_strjoin(paths[i], cmd);
		if (!buf)
			continue ;
		if (stat(buf, &st) == 0 && (st.st_mode & S_IXUSR))
			return (buf);
		else
			ft_free_str(&buf);
	}
	return (NULL);
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
	{
		execute_ast(&(*node)->right_node, mnsh);
	}
		
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
