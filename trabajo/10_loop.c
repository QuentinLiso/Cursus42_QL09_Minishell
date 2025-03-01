#include "minishell.h"

void	prompt_mnsh(t_mnsh *mnsh, bool tester);

int	loop_mnsh(t_mnsh *mnsh)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putstr_fd(MINISHELL, STDOUT_FILENO);
	while (1)
	{
		free_reset_mnsh(mnsh);
		set_signal_interactive();
		prompt_mnsh(mnsh, true);
		if (!mnsh->prompt)
			break ;
		mnsh->line_count++;
		add_history(mnsh->prompt);
		if (strtok_mnsh(mnsh, mnsh->prompt) < 0)
			continue ;
		if (ast_mnsh(&mnsh->node, mnsh->tokis, mnsh->last_tokis, mnsh))
			continue ;
		if (execute_ast(&mnsh->node, mnsh))
			continue ;
	}
	return (mnsh->last_exit_status);
}

void	prompt_mnsh(t_mnsh *mnsh, bool tester)
{
	if (tester)
	{
		mnsh->prompt = readline(MINISHELL_PROMPT);
		return ;
	}
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))			// ./minishell
		mnsh->prompt = readline(MINISHELL_PROMPT);
	else if (isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO))	// ./minishell | bash
		mnsh->prompt = readline(NULL);
	else if (!isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))	// echo echo salut | ./minishell
		mnsh->prompt = get_next_line(STDIN_FILENO);
	else if (!isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO))	// ./minishell | ./minishell | bash
		mnsh->prompt = get_next_line(STDIN_FILENO);
}