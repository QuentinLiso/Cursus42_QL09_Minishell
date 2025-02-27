#include "minishell.h"

void	print_minishell_header()
{
	ft_printf(MINISHELL);
}

int		mnsh_prompt(char **prompt)
{
	*prompt = NULL;

	// if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	*prompt = readline(MINISHELL_PROMPT);
	// else if (isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO))
	// {
	// 	ft_putendl_fd("minishell: error: is not a TTY", STDERR_FILENO);
	// 	exit (1);
	// }
	// else if (isatty(STDOUT_FILENO) && !isatty(STDIN_FILENO))
	// {
	// 	ft_putendl_fd("minishell: error: is not a TTY", STDERR_FILENO);
	// 	exit (1);
	// }
	// else
	// {
	// 	ft_putendl_fd("minishell: error: is not a TTY", STDERR_FILENO);
	// 	exit (1);
	// }
	// if (!*prompt)
	// {
	// 	load_message(14, "☑️  EXIT SUCCESSFUL ☑️\tSee you later :)", 120000);
	// 	exit(0);
	// }
	return (0);	
}