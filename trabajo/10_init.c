#include "minishell.h"

int		mnsh_initialization(t_mnsh *mnsh, int ac, char **env)
{
	int	status;

	init_mnsh_struct(mnsh);
	if (ac > 1)
		return (EINVAL);
	status = set_mnsh_env(mnsh, &mnsh->env_mnsh_lst, env);
	if (status != 0)
	{
		mnsh->last_exit_status = errno_to_exit(status);
		return (mnsh->last_exit_status);
	}
	init_sigaction(&mnsh->sa, &handle_signal);
	return (0);
}

void	init_mnsh_struct(t_mnsh *mnsh)
{
	mnsh->env_mnsh_lst = NULL;
	mnsh->paths = NULL;
	mnsh->prompt = NULL;
	mnsh->tokis = NULL;
	mnsh->last_tokis = NULL;
	mnsh->node = NULL;
	mnsh->last_exit_status = 0;
	mnsh->last_cmd_arg = NULL;
}

int		set_mnsh_env(t_mnsh *mnsh, t_list **env_lst, char **env)
{
	t_list	*new_elem;
	t_var	*var;

	*env_lst = NULL;
	if (!env || !*env)
		return (set_mnsh_empty_env(env_lst));		// ret value
	while (*env)
	{
		var = alloc_env_var(*env);
		if (var)
			new_elem = ft_lstnew(var);
		if (!var || !new_elem)
		{
			ft_lstclear(env_lst, &free_env_var);
			return (free_env_var_ret(var, 1));    // ret value
		}
		ft_lstadd_back(env_lst, new_elem);
		env++;
	}
	if (set_mnsh_env_shlvl(env_lst))
		return (1);									// ret value
	return (set_mnsh_paths(*env_lst, &mnsh->paths));	// ret value
}

int		set_mnsh_empty_env(t_list **env_lst)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 2);
	if (!cwd)
		return (errno);
	if (add_env_var(env_lst, "PWD", cwd))
		return (ENOMEM);
	if (add_env_var(env_lst, "SHLVL", "1"))
		return (ENOMEM);
	if (add_env_var(env_lst, "_", "PATH/TO/MNSH"))
		return (ENOMEM);
	return (0);
}

int		set_mnsh_env_shlvl(t_list **env_lst)
{
	char	*shlvl;

	shlvl = get_env_var(*env_lst, "SHLVL");
	if (shlvl)
	{
		shlvl = ft_itoa(ft_atoi(shlvl) + 1);
		if (!shlvl || edit_env_var(env_lst, "SHLVL", shlvl))
		{
			if (!shlvl)
				ft_putendl_fd("SHLVL MALLOC ERROR", STDERR_FILENO);
			ft_lstclear(env_lst, &free_env_var);
			return (ENOMEM);
		}
		ft_free_str(&shlvl);
	}
	else if (edit_env_var(env_lst, "SHLVL", "1"))
	{
		ft_lstclear(env_lst, &free_env_var);
		return (ENOMEM);
	}
	return (0);
}
