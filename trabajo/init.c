#include "minishell.h"

t_error	mnsh_initialization(int ac, char **av, char **env, t_mnsh *mnsh)
{
	if (ac > 1 || av[1] || !env || !*env)
		return (mnsh_perror(ERR_ARGS)); 
	init_mnsh_struct(mnsh);
	if (duplicate_env(env, ft_strarrlen(env), &mnsh->env_mnsh))
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
	// mnsh->tokens = ft_calloc(128, sizeof(char *));
	// mnsh->tokis = ft_calloc(128, sizeof(t_token));
	// i = -1;
	// while (++i < 128)
	// {
	// 	mnsh->tokis[i].word = NULL;
	// 	mnsh->tokis[i].type = 0;
	// }
	// mnsh->tokis[0].word = ft_strdup("bonjour");
	mnsh->tokis = NULL;
	mnsh->last_tokis = NULL;
	mnsh->node = NULL;
	mnsh->last_exit_status = 0;
}

t_error	duplicate_env(char **env_src, int len, char ***env_dst)
{
	int	i;

	i = -1;
	*env_dst = ft_calloc(len + 2, sizeof(char *));
	if (!*env_dst)
		return (ERR_MALLOC);
	while (env_src[++i])
	{
		(*env_dst)[i] = ft_strdup(env_src[i]);
		if ((*env_dst)[i] == NULL)
		{
			ft_free_strarray(env_dst);
			return (ERR_MALLOC);
		}
	}
	(*env_dst)[i] = NULL;
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
	{
		free(path);
		return (ERR_MALLOC);
	}
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
	free(path);            
	return (ERR_NOERR);
}

/*
Gets the value of an env var
*/
char	*ft_get_env_var(char **env, char *var)
{
	int	i;
	size_t	len_var;
	char	*str;

	i = -1;
	
	if (!env || !env[0] || !var)
		return (ft_strdup(""));
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
	return (ft_strdup(""));
}

/*
Returns the index of the env var
*/
int		ft_get_env_var_index(char **env, char *var)
{
	int	i;
	size_t	len_var;

	i = -1;
	if (!env || !env[0] || !var)
		return (-1);
	len_var = ft_strlen(var);
	(void)len_var;
	while (env[++i])
	{
		if (ft_strncmp(env[i], var, len_var) == 0 && env[i][len_var] == '=')
			return (i);
	}
	return (-1);
}

t_error	ft_reset_env_var(char ***env, char *var, char *value)
{
	int		i;
	char	*new_env_var;

	i = ft_get_env_var_index(*env, var);
	if (i == -1)
		return (ERR_ENV);
	if (!value)
		return (ERR_NOERR);
	new_env_var = ft_strjoin_multi(3, var, "=", value);
	if (!new_env_var)
		return (perror_malloc("ft_reset_env_var"));
	free((*env)[i]);
	(*env)[i] = new_env_var;
	return (ERR_NOERR);
}

t_error	ft_reset_env_var_index(char ***env, char *var, char *value, int i)
{
	char	*new_env_var;

	if (i == -1)
		return (ERR_ENV);
	if (!value)
		return (ERR_NOERR);
	new_env_var = ft_strjoin_multi(3, var, "=", value);
	if (!new_env_var)
		return (perror_malloc("ft_reset_env_var"));
	free((*env)[i]);
	(*env)[i] = new_env_var;
	return (ERR_NOERR);
}

t_error	ft_new_env_var(char ***env, char *var, char *value)
{
	char	**new_env;
	int		env_len;

	env_len = ft_strarrlen(*env);
	(void)var;(void)value;
	if (duplicate_env(*env, env_len + 2, &new_env))
		return (perror_malloc("ft_new_env_var"));
	if (!value)
		new_env[env_len] = ft_strdup(var);
	else
		new_env[env_len] = ft_strjoin_multi(3, var, "=", value);
	if (!new_env[env_len])
	{
		ft_free_strarray(&new_env);
		return (perror_malloc("ft_new_env_var"));
	}
	new_env[env_len + 1] = NULL;
	ft_free_strarray(env);
	*env = new_env;
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