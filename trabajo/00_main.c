#include "minishell.h"

int	loop_mnsh(t_mnsh *mnsh)
{
	int	status;

	ft_free_reset_mnsh(mnsh);
	status = mnsh_prompt(&mnsh->prompt);
	if (status)
		return (status);
	add_history(mnsh->prompt);
	if (strtok_mnsh(mnsh, mnsh->prompt) < 0)
		return (mnsh->last_exit_status);
	if (ast_mnsh(&mnsh->node, mnsh->tokis, mnsh->last_tokis, mnsh))
		return (mnsh->last_exit_status);
	print_ast(mnsh->node, 0);
	if (execute_ast(&mnsh->node, mnsh))
		return (mnsh->last_exit_status);
	return (0);
}

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;

	// if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	// 	print_minishell_header();
	if (mnsh_initialization(&mnsh, ac, env))
	{
		ft_free_all_mnsh(&mnsh);
		return (mnsh.last_exit_status);
	}
	while (1)
		loop_mnsh(&mnsh);
	(void)ac; (void)av; (void)env;
	return (EXIT_SUCCESS);
}