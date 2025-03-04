/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   30_ast_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:59:32 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 12:17:27 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	loop_heredoc(int fd_wr, int *count, char *heredoc_end, t_mnsh *mnsh)
{
	char	*prompt;

	while (1)
	{
		prompt = readline("heredoc> ");
		if (g_signal_received == SIGINT)
		{
			free_str(&prompt);
			return (130);
		}
		if (!prompt)
		{
			warn_heredoc(mnsh->line_count, heredoc_end);
			break ;
		}
		(*count)++;
		if (!ft_strcmp(prompt, heredoc_end))
			break ;
		ft_putendl_fd(prompt, fd_wr);
		free_str(&prompt);
	}
	free_str(&prompt);
	return (0);
}

void	warn_heredoc(int line, char *heredoc_end)
{
	ft_putstr_fd("minishell: warning: here-document at line ", STDERR_FILENO);
	ft_putnbr_fd(line, STDERR_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(heredoc_end, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

int	parent_heredoc(int count_pipe[2], pid_t pid, t_mnsh *mnsh)
{
	int		status;
	int		count;

	close(count_pipe[1]);
	set_sigaction(&mnsh->sa_sigint, SIGINT, SIG_IGN, 0);
	waitpid(pid, &status, 0);
	if (read(count_pipe[0], &count, sizeof(int)) < (ssize_t) sizeof(int)
		|| count < 0)
	{
		perror_mnsh(1, 1, "Error while counting lines");
		count = 0;
	}
	increment_mnsh_line_count(mnsh, count);
	close(count_pipe[0]);
	set_sigaction(&mnsh->sa_sigint, SIGINT, h_sigint_loop, 0);
	return (WEXITSTATUS(status));
}

int	set_cmdnode_args(t_ast **node, t_token *start, t_token *end)
{
	t_token	*iterator;
	int		i;

	(*node)->args = ft_calloc(cmd_args_count(start, end) + 1, sizeof(char *));
	if (!(*node)->args)
		return (perror_mnsh(ENOMEM, 1, "malloc err in set args"));
	iterator = start;
	i = 0;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
			iterator = iterator->next;
		else
		{
			(*node)->args[i] = ft_strdup(iterator->word);
			if (!(*node)->args[i++])
				return (perror_mnsh(ENOMEM, 1, "malloc err in set args"));
		}
		iterator = iterator->next;
	}
	return (0);
}

int	cmd_args_count(t_token *start, t_token *end)
{
	t_token		*iterator;
	int			len;

	iterator = start;
	len = 0;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
		{
			iterator = iterator->next->next;
			continue ;
		}
		iterator = iterator->next;
		len++;
	}
	return (len);
}
