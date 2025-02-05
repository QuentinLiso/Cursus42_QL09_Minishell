#include "minishell.h"

t_error	mnsh_initialization(int ac, char **av, char **env, t_mnsh *mnsh)
{
	if (ac > 1 || av[1] || !env || !*env)
		return (mnsh_perror(ERR_ARGS)); 
	init_mnsh_struct(mnsh);
	if (duplicate_env(env, &mnsh->env_mnsh))
		return (mnsh_perror(ERR_ENV));
	if (set_mnsh_paths(mnsh->env_mnsh, &mnsh->paths))
		return (mnsh_perror(ERR_ENV));
	init_sigaction(&mnsh->sa, &handle_signal);
	return (ERR_NOERR);
}

void	init_mnsh_struct(t_mnsh *mnsh)
{
	mnsh->env_mnsh = NULL;
	mnsh->paths = NULL;
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

t_error	set_mnsh_paths(char **env_mnsh, char ***paths)
{
	char	*path;
	char	*buf;
	int		i;

	path = ft_get_env_var(env_mnsh, "PATH");
	if (!path)
		return (ERR_MALLOC);
	*paths = ft_split(path, ':');
	if (!*paths)
		return (ERR_MALLOC);
	i = -1;
	while ((*paths)[++i])
	{
		if ((*paths)[i][ft_strlen((*paths)[i]) - 1] != '/')
		{
			buf = ft_strjoin((*paths)[i], "/");
			free((*paths)[i]);
			(*paths)[i] = buf;
		}
	}                
	return (ERR_NOERR);
}

char	*ft_get_env_var(char **env, char *var)
{
	int	i;
	size_t	len_var;
	char	*str;

	i = -1;
	
	if (!env || !env[0] || !var)
		return (NULL);
	len_var = ft_strlen(var);
	(void)len_var;
	while (env[++i])
	{
		if (ft_strncmp(env[i], var, len_var) == 0 && env[i][len_var] == '=')
		{
			str = ft_strdup(&env[i][len_var + 1]);
			if (!str)
				return (NULL);
			return (str);
		}	
	}
	return (NULL);
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