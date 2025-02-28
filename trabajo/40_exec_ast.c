#include "minishell.h"

int		execute_ast(t_ast **node, t_mnsh *mnsh)
{
	if (!(*node) || !(*node)->args || !(*node)->args[0])
		return (0);
	if ((*node)->node_type == NODE_CMD)
	{
		if (set_mnsh_last_arg(node, mnsh))
		{
			mnsh->last_exit_status = errno_to_exit(ENOMEM);
			return (mnsh->last_exit_status);
		}
		mnsh->last_exit_status = exec_ast_cmd(node, mnsh);
		return (mnsh->last_exit_status);
	}		
	else if ((*node)->node_type == NODE_OP)
	{
		mnsh->last_exit_status = exec_ast_op(node, (*node)->op_type, mnsh);
		return (mnsh->last_exit_status);
	}
	return (0);
}

int		set_mnsh_last_arg(t_ast **node, t_mnsh *mnsh)
{
	int		len;

	len = ft_strarrlen((*node)->args);
	if (len < 2)
		return (0);
	return (edit_env_var(&mnsh->env_mnsh_lst, "_", (*node)->args[len - 1]));
}

int		exec_ast_cmd(t_ast **node, t_mnsh *mnsh)
{
	int		default_inout[2];
	int		status;

	status = init_default_fd(&default_inout);
	if (status)
		return (status);
	status = exec_ast_cmd_indir((*node)->redir);
	if (status)
	{
		clean_default_fd(&default_inout);
		return (status);
	}
	if (is_builtin((*node)->args[0]))
		status = exec_ast_cmd_builtin((*node)->args, mnsh);
	else
		status = exec_ast_cmd_external((*node)->args, mnsh);
	if (status)
	{
		clean_default_fd(&default_inout);
		return (status);
	}
	return (clean_default_fd(&default_inout));
}

int		init_default_fd(int (*default_inout)[2])
{
	(*default_inout)[0] = dup(STDIN_FILENO);
	if ((*default_inout)[0] < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	(*default_inout)[1] = dup(STDOUT_FILENO);
	if ((*default_inout)[1] < 0)
	{
		close((*default_inout)[0]);
		return (perror_mnsh(1,1, strerror(errno)));
	}
	return (0);
}

int		clean_default_fd(int (*default_inout)[2])
{
	int	status;

	status = 0;
	if (dup2((*default_inout)[0], STDIN_FILENO) < 0)
	{
		perror_mnsh(1, 1, strerror(errno));
		status = 1;
	}
	close((*default_inout)[0]);
	(*default_inout)[0] = -1;
	if (dup2((*default_inout)[1], STDOUT_FILENO) < 0)
	{
		perror_mnsh(1,1, strerror(errno));
		status = 1;
	}
	close((*default_inout)[1]);
	(*default_inout)[1] = -1;
	return (status);
}


int		exec_ast_cmd_indir(t_list *redir)
{
	int	status;

	status = check_access_indir_lst(redir);
	if (status)
		return (status);
	status = dup_indir_lst(redir);
	if (status)
		return (status);
	return (0);
}

int		check_access_indir_lst(t_list *redir)
{
	t_redir	*elem;
	int		status;

	while (redir)
	{
		elem = (t_redir *)redir->content;
		status = check_access_indir_elem(elem);
		if (status)
			return (status);
		redir = redir->next;
	}
	return (0);
}

int		check_access_indir_elem(t_redir *elem)
{
	struct stat	st;

	if (elem && elem->file &&
		((elem->style == REDIR_IN) || (elem->style == REDIR_HEREDOC)))
	{
		if (access(elem->file, R_OK) < 0)
			return (perror_mnsh(1, 2, elem->file, strerror(errno)));
	}
	else if (elem && elem->file &&
			((elem->style == REDIR_OUT) || (elem->style == REDIR_APPEND)))
	{
		if (!stat(elem->file, &st))
		{
			if (access(elem->file, W_OK) < 0)
				return (perror_mnsh(1, 2, elem->file, strerror(errno)));
			return (0);
		}
		return (create_outfile(elem));
	}
	return (0);
}

int		create_outfile(t_redir *redir_file)
{
	int	flag;
	int	fd;

	if (redir_file->style == REDIR_APPEND)
		flag = O_APPEND;
	else
		flag = O_TRUNC;
	fd = open(redir_file->file, O_WRONLY | O_CREAT | flag, 0644);
	if (fd < 0)
		return (perror_mnsh(errno_to_exit(errno), 2, redir_file->file,
			strerror(errno)));
	close(fd);
	return (0);
}

int		dup_indir_lst(t_list *redir)
{
	t_redir	*elem;
	int		status;
	int		fd_in;
	int		fd_out;

	fd_in = -1;
	fd_out = -1;
	while (redir)
	{
		elem = (t_redir *)redir->content;
		status = dup_indir_elem(elem, &fd_in, &fd_out);
		if (status)
		{
			if (fd_in > -1)
				close(fd_in);
			if (fd_out > -1)
				close(fd_out);
			return (status);
		}
		redir = redir->next;
	}
	return (0);
}

int		dup_indir_elem(t_redir *elem, int *fd_in, int *fd_out)
{
	if (!elem || !elem->file)
		return (0);
	if	((elem->style == REDIR_IN) || (elem->style == REDIR_HEREDOC))
		return (dup_indir_elem_in(elem->file, fd_in));
	else if (elem->style == REDIR_OUT)
		return (dup_indir_elem_out(elem->file, fd_out, O_TRUNC));
	else if (elem->style == REDIR_APPEND)
		return (dup_indir_elem_out(elem->file, fd_out, O_APPEND));
	return (0);
}

int		dup_indir_elem_in(char *file, int *fd_in)
{
	*fd_in = open(file, O_RDONLY);
	if (*fd_in < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	if (dup2(*fd_in, STDIN_FILENO) < 0)
	{
		close(*fd_in);
		return (perror_mnsh(1, 1, strerror(errno)));
	}
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = -1;
	return (0);
}

int		dup_indir_elem_out(char *file, int *fd_out, int flag)
{
	*fd_out = open(file, O_WRONLY | O_CREAT | flag, 0644);
	if (*fd_out < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	if (dup2(*fd_out, STDOUT_FILENO) < 0)
	{
		close(*fd_out);
		return (perror_mnsh(1, 1, strerror(errno)));
	}
	close(*fd_out);
	*fd_out = -1;
	return (0);
}


int		exec_ast_cmd_external(char **args, t_mnsh *mnsh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (perror_mnsh(1, 1, "fork failed"));
	else if (pid == 0)
		exit(check_and_execute_cmd(args, mnsh));
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}

int		check_and_execute_cmd(char **args, t_mnsh *mnsh)
{
	char		*execfile;
	int			status;

	if (!args || !args[0])
		return (0);
	status = set_execfile(&execfile, args, mnsh);
	if (status == ENOMEM)
		return (status);
	status = check_execfile(execfile, args);
	if (status)
	{
		ft_free_str(&execfile);
		return (status);
	}
	return (ft_execve(&execfile, args, mnsh));
}

int		set_execfile(char **execfile, char **args, t_mnsh *mnsh)
{
	*execfile = NULL;
	if (*args[0] && !ft_strchr(args[0], '/'))
		return (get_cmd_path(execfile, args[0], mnsh->paths));
	else
	{
		*execfile = ft_strdup(args[0]);
		if (!*execfile)
			return (perror_mnsh(ENOMEM, 1, "err malloc in set exec file"));
	}
	return (0);
}

int		check_execfile(char *execfile, char **args)
{
	struct stat st;
	
	if (!execfile || *execfile == '\0')
		return (perror_mnsh(127, 2, args[0], "command not found"));
	if (stat(execfile, &st) == -1)
		return (perror_mnsh(127, 2, args[0], strerror(ENOENT)));
	if (S_ISDIR(st.st_mode))
		return (perror_mnsh(126, 2, args[0], strerror(EISDIR)));
	if (!(S_IXUSR & st.st_mode))
		return (perror_mnsh(126, 2, args[0], strerror(EACCES)));
	return (0);
}

int		get_cmd_path(char **execfile, char *cmd, char **paths)
{
	int		i;
	char 	*buf;
	struct stat	st;

	i = -1;
	while (paths[++i])
	{
		buf = ft_strjoin(paths[i], cmd);
		if (!buf)
			return (perror_mnsh(ENOMEM, 1, "err malloc get cmd path"));
		if (stat(buf, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			*execfile = buf;
			return (0);
		}
		else
			ft_free_str(&buf);
	}
	return (ENOENT);
}

int		ft_execve(char **execfile, char **args, t_mnsh *mnsh)
{
	char	**env;
	int		status;

	env = env_lst_to_arr(mnsh->env_mnsh_lst);
	if (!env)
		return (perror_mnsh(12, 1, "err malloc env for exec"));
	status = execve(*execfile, args, env);
	printf("Exec : %d\n", status);
	ft_free_strarray(&env);
	ft_free_str(execfile);	
	if (errno == ENOENT)
		return (perror_mnsh(127, 2, args[0], strerror(ENOENT)));
	else if (errno == EACCES)
		return (perror_mnsh(126, 2, args[0], strerror(EACCES)));
	else
		return (perror_mnsh(1, 2, args[0], strerror(errno)));
}

int	exec_ast_op(t_ast **node, t_optype op, t_mnsh *mnsh)
{
	if (op == OP_AND)
		return (exec_ast_op_and(node, mnsh));
	else if (op == OP_OR)
		return (exec_ast_op_or(node, mnsh));
	else if (op == OP_PIPE)
		return (exec_ast_op_pipe(node, mnsh));
	else
		return (0);
}

int	exec_ast_op_and(t_ast **node, t_mnsh *mnsh)
{
	int	status;

	status = execute_ast(&((*node)->left_node), mnsh);
	if (mnsh->last_exit_status == 0)
		return (execute_ast(&(*node)->right_node, mnsh));
	return (status);
}

int	exec_ast_op_or(t_ast **node, t_mnsh *mnsh)
{
	int	status;

	status = execute_ast(&((*node)->left_node), mnsh);
	if (mnsh->last_exit_status != 0)
		return (execute_ast(&(*node)->right_node, mnsh));
	return (status);
}

int	exec_ast_op_pipe(t_ast **node, t_mnsh *mnsh)
{
	int		fd[2];
	pid_t	pid[2];
	int		status[2];

	if (pipe(fd) < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	if (left_pipe(node, &fd, &pid[0], mnsh) < 0)
		return (1);
	if (right_pipe(node, &fd, &pid[1], mnsh) < 0)
		return (1);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);
	return (0);
}

int	left_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh)
{
	*pid = fork();
	if (*pid < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	else if (*pid == 0)
	{
		close((*fd)[0]);
		if (dup2((*fd)[1], STDOUT_FILENO) < 0)
			exit(perror_mnsh(1, 1, strerror(errno)));
		close((*fd)[1]);
		exit(execute_ast(&(*node)->left_node, mnsh));
	}
	return (0);
}

int	right_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh)
{
	*pid = fork();
	if (*pid < 0)
		return (perror_mnsh(1, 1, strerror(errno)));
	else if (*pid == 0)
	{
		close((*fd)[1]);
		if (dup2((*fd)[0], STDIN_FILENO) < 0)
			exit(perror_mnsh(1, 1, strerror(errno)));
		close((*fd)[0]);
		exit(execute_ast(&(*node)->right_node, mnsh));
	}
	return (0);
}
