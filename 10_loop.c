/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:46:16 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 15:46:42 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	loop_mnsh(t_mnsh *mnsh)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putstr_fd(MINISHELL, STDOUT_FILENO);
	while (1)
	{
		free_reset_mnsh(mnsh);
		set_sigaction(&mnsh->sa_sigint, SIGINT, h_sigint_loop, 0);
		set_sigaction(&mnsh->sa_sigquit, SIGQUIT, SIG_IGN, 0);
		prompt_mnsh(mnsh, true);
		if (!mnsh->prompt)
			break ;
		check_sigint_loop(mnsh);
		increment_mnsh_line_count(mnsh, 1);
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
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		mnsh->prompt = readline(MINISHELL_PROMPT);
	else if (isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO))
		mnsh->prompt = readline(NULL);
	else if (!isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		mnsh->prompt = get_next_line(STDIN_FILENO);
	else if (!isatty(STDIN_FILENO) && !isatty(STDOUT_FILENO))
		mnsh->prompt = get_next_line(STDIN_FILENO);
}

void	check_sigint_loop(t_mnsh *mnsh)
{
	if ((g_signal_received & 0xFF) == SIGINT)
	{
		mnsh->last_exit_status = 130;
		increment_mnsh_line_count(mnsh, (g_signal_received >> 8) - 1);
		mnsh->line_count++;
		g_signal_received = 0;
	}
}