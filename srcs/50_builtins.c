/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   50_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:17:33 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:50 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *s)
{
	if (!s)
		return (false);
	else if (ft_strcmp(s, "echo") == 0)
		return (true);
	else if (ft_strcmp(s, "cd") == 0)
		return (true);
	else if (ft_strcmp(s, "pwd") == 0)
		return (true);
	else if (ft_strcmp(s, "export") == 0)
		return (true);
	else if (ft_strcmp(s, "unset") == 0)
		return (true);
	else if (ft_strcmp(s, "env") == 0)
		return (true);
	else if (ft_strcmp(s, "exit") == 0)
		return (true);
	return (false);
}

int	exec_ast_cmd_builtin(char **args, t_mnsh *mnsh)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (mnsh_echo(args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (mnsh_cd(args, mnsh));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (mnsh_pwd());
	else if (ft_strcmp(args[0], "export") == 0)
		return (mnsh_export(args, mnsh));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (mnsh_unset(args, mnsh));
	else if (ft_strcmp(args[0], "env") == 0)
		return (mnsh_env(mnsh));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (mnsh_exit(args, mnsh));
	return (0);
}

int	mnsh_echo(char **args)
{
	int		i;
	bool	option;

	if (args[1] == NULL)
		printf("\n");
	else
	{
		i = 0;
		option = false;
		while (args[++i])
			if (!is_echo_option_valid(args[i]))
				break ;
		else
		option = true;
		print_strarray_raw(&args[i], ' ');
		if (!option)
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}

bool	is_echo_option_valid(char *arg)
{
	int	i;

	if (ft_strncmp(arg, "-n", 2) == 0)
	{
		i = 1;
		while (arg[++i])
			if (arg[i] != 'n')
				return (false);
		return (true);
	}
	return (false);
}

int	mnsh_env(t_mnsh *mnsh)
{
	t_list	*iterator;
	t_var	*var;

	iterator = mnsh->env_mnsh_lst;
	while (iterator)
	{
		var = (t_var *)iterator->content;
		if (var->value)
		{
			ft_putstr_fd(var->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(var->value, STDOUT_FILENO);
		}
		iterator = iterator->next;
	}
	return (0);
}
