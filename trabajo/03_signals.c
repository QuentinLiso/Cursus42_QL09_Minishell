#include "minishell.h"

void	init_sigaction(t_sa *sa, void (*action)(int, siginfo_t *, void *))
{
	ft_bzero(sa, sizeof(sa));
	sa->sa_sigaction = action;
	sigemptyset(&(sa->sa_mask));
	sa->sa_flags = SA_SIGINFO | SA_RESTART;
	// sigaction(SIGINT, sa, NULL);
	// signal(SIGQUIT, SIG_IGN);
}

void	handle_signal(int signum, siginfo_t *info, void *other)
{
	(void)info;
	(void)other;
	if (signum == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
		
}