/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   50_builtins_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:19:29 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 16:20:31 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	mnsh_export(char **args, t_mnsh *mnsh)
{
	int	i;
	int	status;

	status = 0;
	if (args[1] == NULL)
		return (print_export_var(mnsh->env_mnsh_lst));
	else
	{
		i = 0;
		while (args[++i])
			status = handle_export_var(args[i], mnsh);
	}
	return (status);
}

int	print_export_var(t_list *env)
{
	t_var	*var;

	if (!env)
		return (perror_mnsh(1, 1, "ENV not available"));
	while (env)
	{
		var = (t_var *)env->content;
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(var->key, STDOUT_FILENO);
		if (var->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(var->value, STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		env = env->next;
	}
	return (0);
}

int	handle_export_var(char *arg, t_mnsh *mnsh)
{
	size_t	i;
	char	*key;
	char	*value;
	int		status;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (perror_mnsh(1, 3, "export", arg, "not a valid identifier"));
	i = 0;
	while(arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (perror_mnsh(1, 3, "export", arg, "not a valid identifier"));
		i++;
	}
	status = set_export_key_value(arg, i, &key, &value);
	if (status)
		return (status);
	status = update_export_var(key, value, mnsh);
	free_str(&key);
	free_str(&value);
	return (status);
}

int	set_export_key_value(char *arg, int i, char **key, char **value)
{
	*key = ft_substr(arg, 0, i);
	if (!*key)
		return (perror_mnsh(ENOMEM, 1, "malloc err in set export key val"));
	if (!arg[i])
		*value = NULL;
	else
	{
		*value = ft_strdup(&arg[i + 1]);
		if (!*value)
		{
			free_str(key);
			return (perror_mnsh(ENOMEM, 1, "malloc err in set export key val"));
		}
	}
	return (0);
}

int	update_export_var(char *key, char *value, t_mnsh *mnsh)
{
	char	*env_var_val;

	env_var_val = get_env_var(mnsh->env_mnsh_lst, key);
	if (!value)
	{
		if (!env_var_val)
			return (add_env_var(&mnsh->env_mnsh_lst, key, value));
		return (0);
	}
	else
		return (edit_env_var(&mnsh->env_mnsh_lst, key, value));
}