/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:46:16 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:41:00 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	loop_mnsh(t_mnsh *mnsh)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		print_header();
	while (1)
	{
		if (mnsh->tty_loop)
			break ;
		free_reset_mnsh(mnsh);
		set_sigaction(&mnsh->sa_sigint, SIGINT, h_sigint_loop, 0);
		set_sigaction(&mnsh->sa_sigquit, SIGQUIT, SIG_IGN, 0);
		prompt_mnsh(mnsh);
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

void	print_header(void)
{
	ft_printf("\033[34m===================================");
	ft_printf("======================================\n\n");
	ft_printf("__  __   ___   _   _   ___     ");
	ft_printf("___    _   _   _____   _       _       \n");
	ft_printf("|  \\/  | |_ _| | \\ | | |_ _|   ");
	ft_printf("/ ___|  | | | | | ____| | |     | |      \n");
	ft_printf("| |\\/| |  | |  |  \\| |  | |    ");
	ft_printf("\\___ \\  | |_| | |  _|   | |     | |      \n");
	ft_printf("| |  | |  | |  | |\\  |  | |     ");
	ft_printf("___) | |  _  | | |___  | |___  | |___   \n");
	ft_printf("|_|  |_| |___| |_| \\_| |___|   ");
	ft_printf("|____/  |_| |_| |_____| |_____| |_____|  \n\n\n");
	ft_printf("========================================");
	ft_printf("===================================\n\n\033[0m");
}

void	prompt_mnsh(t_mnsh *mnsh)
{
	char	*gnl;

	if (isatty(STDIN_FILENO))
		mnsh->prompt = readline("\033[34m 🐚 Minishell 🐚 > \033[0m");
	else
	{
		gnl = get_next_line(STDIN_FILENO);
		mnsh->prompt = ft_strtrim(gnl, "\n");
		free_str(&gnl);
		mnsh->tty_loop = true;
	}
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
