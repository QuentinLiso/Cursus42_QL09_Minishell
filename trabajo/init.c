#include "minishell.h"

t_error	mnsh_initialization(int ac, char **av, char **env, t_mnsh *mnsh)
{
	if (ac > 1 || av[1] || !env || !*env)
		return (mnsh_perror(ERR_ARGS)); 
	init_mnsh_struct(mnsh);
	if (duplicate_env(env, &mnsh->env_mnsh))
		return (mnsh_perror(ERR_ENV));
	init_sigaction(&mnsh->sa, &handle_signal);
	return (ERR_NOERR);
}

void	init_mnsh_struct(t_mnsh *mnsh)
{
	mnsh->env_mnsh = NULL;
	mnsh->prompt = NULL;
	mnsh->last_exit_status = 0;
}

t_error	duplicate_env(char **env, char ***env_mnsh)
{
	int	i;
	int	len;

	i = -1;
	len = -1;
	while (env[++len])
		;
	*env_mnsh = ft_calloc(len + 2, sizeof(char *));
	if (!*env_mnsh)
		return (ERR_MALLOC);
	while (env[++i])
	{
		(*env_mnsh)[i] = ft_strdup(env[i]);
		if ((*env_mnsh)[i] == NULL)
			return (ERR_MALLOC);
	}
	(*env_mnsh)[i] = NULL;
	return (ERR_NOERR);
}

void	init_sigaction(t_sa *sa, void (*action)(int, siginfo_t *, void *))
{
	ft_bzero(sa, sizeof(sa));
	sa->sa_sigaction = action;
	sigemptyset(&(sa->sa_mask));
	sa->sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGQUIT, sa, NULL);
}

void	handle_signal(int signum, siginfo_t *info, void *other)
{
	(void)info;
	(void)other;
	if (signum == SIGQUIT)
		ft_printf("Caught SIGQUIT\n");
}

void	print_minishell_header()
{
	ft_printf(MINISHELL);
}