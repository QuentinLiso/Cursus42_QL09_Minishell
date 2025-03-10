/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:34:42 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 17:01:02 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mnsh_initialization(t_mnsh *mnsh, int ac, char **env)
{
	int	status;

	init_mnsh_struct(mnsh);
	if (ac > 1)
		return (EINVAL);
	status = set_mnsh_env(&mnsh->env_mnsh_lst, env);
	if (status != 0)
	{
		mnsh->last_exit_status = errno_to_exit(status);
		return (mnsh->last_exit_status);
	}
	return (0);
}

void	init_mnsh_struct(t_mnsh *mnsh)
{
	mnsh->env_mnsh_lst = NULL;
	mnsh->prompt = NULL;
	mnsh->tokis = NULL;
	mnsh->last_tokis = NULL;
	mnsh->node = NULL;
	mnsh->last_exit_status = 0;
	mnsh->last_cmd_arg = NULL;
	mnsh->line_count = 0;
	mnsh->tty_loop = false;
	mnsh->tok_operators[0] = "&&";
	mnsh->tok_operators[1] = "||";
	mnsh->tok_operators[2] = "|";
	mnsh->tok_operators[3] = NULL;
	mnsh->tok_indir[0] = "<>";
	mnsh->tok_indir[1] = "<<";
	mnsh->tok_indir[2] = ">>";
	mnsh->tok_indir[3] = "<";
	mnsh->tok_indir[4] = ">";
	mnsh->tok_indir[5] = NULL;
	mnsh->env_specials = "#*@$!0123456789-";
}

int	set_mnsh_env(t_list **env_lst, char **env)
{
	t_list	*new_elem;
	t_var	*var;

	*env_lst = NULL;
	if (!env || !*env)
		return (set_mnsh_empty_env(env_lst));
	while (*env)
	{
		var = alloc_env_var(*env);
		if (var)
			new_elem = ft_lstnew(var);
		if (!var || !new_elem)
		{
			ft_lstclear(env_lst, &free_env_var);
			return (free_env_var_ret(var, 1));
		}
		ft_lstadd_back(env_lst, new_elem);
		env++;
	}
	return (set_mnsh_env_shlvl(env_lst));
}

int	set_mnsh_empty_env(t_list **env_lst)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
		return (errno);
	if (add_env_var(env_lst, "PWD", cwd))
		return (ENOMEM);
	if (add_env_var(env_lst, "SHLVL", "1"))
		return (ENOMEM);
	if (add_env_var(env_lst, "_", "minishell"))
		return (ENOMEM);
	return (0);
}

int	set_mnsh_env_shlvl(t_list **env_lst)
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
		free_str(&shlvl);
	}
	else if (edit_env_var(env_lst, "SHLVL", "1"))
	{
		ft_lstclear(env_lst, &free_env_var);
		return (ENOMEM);
	}
	return (0);
}
