#include "minishell.h"

int	loop_mnsh(t_mnsh *mnsh)
{
	// if (isatty(STDIN_FILENO))
	// 	ft_putstr_fd(MINISHELL, STDOUT_FILENO);
	while (1)
	{
		free_reset_mnsh(mnsh);
		if (isatty(STDIN_FILENO))
			mnsh->prompt = readline(MINISHELL_PROMPT);
		else
		{
			// mnsh->prompt = readline(MINISHELL_PROMPT);
			mnsh->prompt = get_next_line(STDIN_FILENO);
			if (mnsh->prompt && mnsh->prompt[0])
				mnsh->prompt[ft_strlen(mnsh->prompt) - 1] = '\0';
		}
		if (!mnsh->prompt)
			break;
		mnsh->line_count++;
		add_history(mnsh->prompt);
		if (strtok_mnsh(mnsh, mnsh->prompt) < 0)
			continue ;
		if (ast_mnsh(&mnsh->node, mnsh->tokis, mnsh->last_tokis, mnsh))
			continue ;
		if (execute_ast(&mnsh->node, mnsh))
			continue ;
		if (!isatty(STDIN_FILENO))
			break ;
	}
	return (mnsh->last_exit_status);
}

