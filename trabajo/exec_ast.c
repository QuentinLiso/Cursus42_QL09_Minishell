#include "minishell.h"

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

void	exec_ast_op_out(t_ast **node, t_mnsh *mnsh)
{
	int	flags;
	int	fd;
	pid_t	pid;

	flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open((*node)->right_node->value, flags, 0644);
	if (fd < 0)
	{
		perror("open failed");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execute_ast(&(*node)->left_node, mnsh);
		exit(0);
	}
	close(fd);
	waitpid(pid, NULL, 0);
}

void	exec_ast_op_outapd(t_ast **node, t_mnsh *mnsh)
{
	int	flags;
	int	fd;
	pid_t	pid;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open((*node)->right_node->value, flags, 0644);
	if (fd < 0)
	{
		perror("open failed");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execute_ast(&(*node)->left_node, mnsh);
		exit(0);
	}
	close(fd);
	waitpid(pid, NULL, 0);
}

void	exec_ast_op_in(t_ast **node, t_mnsh *mnsh)
{
	int		fd;
	pid_t	pid;

	fd = open((*node)->right_node->value, O_RDONLY);
	if (fd < 0)
	{
		perror("open failed");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		if ((*node)->left_node != NULL)
			execute_ast(&(*node)->left_node, mnsh);
		else
			printf("Bidule\n");
		exit(0);
	}
	close(fd);
	waitpid(pid, NULL, 0);
}