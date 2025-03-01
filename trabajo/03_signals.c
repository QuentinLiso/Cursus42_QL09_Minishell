#include "minishell.h"

void	init_sigaction(t_sa *sa, void (*handler)(int))
{
	ft_bzero(sa, sizeof(*sa));
	sigemptyset(&(sa->sa_mask));
	sa->sa_flags = SA_RESTART;
	sa->sa_handler = handler;
	sigaction(SIGINT, sa, NULL);
}

void	handle_signal_interactive(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_signal_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_received = 1;
		close(STDIN_FILENO);
	}
}

void	set_signal_interactive()
{
	t_sa	sa;
	
	signal(SIGQUIT, SIG_IGN);
	init_sigaction(&sa, &handle_signal_interactive);
}

void	set_signal_heredoc()
{
	t_sa	sa;
	
	signal(SIGQUIT, SIG_IGN);
	init_sigaction(&sa, &handle_signal_heredoc);
}