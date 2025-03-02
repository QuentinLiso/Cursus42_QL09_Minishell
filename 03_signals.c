/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:44:57 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 15:46:08 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	set_sigaction(t_sa *sa, int signum, void (*handler)(int), int flags)
{
	sa->sa_flags = flags;
	sa->sa_handler = handler;
	sigemptyset(&sa->sa_mask);
	sigaction(signum, sa, NULL);
}

// =============================

void	h_sigint_loop(__attribute__((unused)) int signum)
{
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// ===============================

void	h_sigint_heredoc(__attribute__((unused)) int signum)
{
	// rl_on_new_line();
	// rl_replace_line("", 0);
	ft_putchar_fd('\n', STDOUT_FILENO);
	// rl_redisplay();
	close(STDIN_FILENO);
	g_signal_received = SIGINT;
}

// =================================

void	h_sigint_cmd(__attribute__((unused)) int signum)
{
	close(STDIN_FILENO);
	g_signal_received = SIGINT;
}

void	h_sigquit_cmd(__attribute__((unused)) int signum)
{
	ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	close(STDIN_FILENO);
	g_signal_received = SIGQUIT;
}