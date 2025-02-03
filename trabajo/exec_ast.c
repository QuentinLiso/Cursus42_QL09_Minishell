#include "minishell.h"

void	exec_ast_cmd(t_ast **node, t_mnsh *mnsh)
{
	pid_t	pid;
	int		fd_inout[2];

	pid = fork();
	if (pid == 0)
	{
		exec_ast_cmd_in(node, &fd_inout[0]);
		exec_ast_cmd_out(node, &fd_inout[1]);
		execve((*node)->args[0], (*node)->args, NULL);
		perror("execve failed");
		exit(127);
	}
	waitpid(pid, &mnsh->last_exit_status, 0);
}

void	exec_ast_cmd_out(t_ast **node, int *fd)
{
	t_outfiles	*iterator;

	iterator = (*node)->outfiles;
	if (!iterator || !iterator->outfile)
		return ;
	while (iterator != NULL)
	{
		printf("1 %s\n", iterator->outfile);
		if (iterator->outstyle == OUT_TRUNC)
		{
			printf("Prems\n");
			exec_ast_cmd_outfile(iterator->outfile, fd, O_TRUNC);
		}
		else if (iterator->outstyle == OUT_APPEND)
		{
			printf("Deuz\n");
			exec_ast_cmd_outfile(iterator->outfile, fd, O_APPEND);
		}
		printf("2 %s\n", iterator->outfile);
		iterator = iterator->next;
		printf("3 %s\n", iterator->outfile);
	}
}

void	exec_ast_cmd_outfile(char *outfile, int *fd, int flag)
{
	printf("toto\n");
	*fd = open(outfile, O_WRONLY | O_CREAT | flag, 0644);
	printf("toto\n");
	if (*fd < 0)
	{
		perror("open failed");
		exit (errno);
	}
	printf("toto\n");
	dup2(*fd, STDOUT_FILENO);
	printf("toto\n");
	close(*fd);
	printf("toto\n");
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

// void	exec_ast_cmd_outfiles(t_ast	**node, t_mnsh *mnsh)
// {

// }

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

// void	exec_ast_op_out(t_ast **node, t_mnsh *mnsh)
// {
// 	int	flags;
// 	int	fd;
// 	pid_t	pid;

// 	flags = O_WRONLY | O_CREAT | O_TRUNC;
// 	fd = open((*node)->right_node->outfiles->outfile, flags, 0644);
// 	if (fd < 0)
// 	{
// 		perror("open failed");
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		dup2(fd, STDOUT_FILENO);
// 		close(fd);
// 		execute_ast(&(*node)->left_node, mnsh);
// 		exit(0);
// 	}
// 	close(fd);
// 	waitpid(pid, NULL, 0);
// }

// void	exec_ast_op_outapd(t_ast **node, t_mnsh *mnsh)
// {
// 	int	flags;
// 	int	fd;
// 	pid_t	pid;

// 	flags = O_WRONLY | O_CREAT | O_APPEND;
// 	fd = open((*node)->right_node->outfiles->outfile, flags, 0644);
// 	if (fd < 0)
// 	{
// 		perror("open failed");
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		dup2(fd, STDOUT_FILENO);
// 		close(fd);
// 		execute_ast(&(*node)->left_node, mnsh);
// 		exit(0);
// 	}
// 	close(fd);
// 	waitpid(pid, NULL, 0);
// }
