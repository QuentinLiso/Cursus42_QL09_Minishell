/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   71_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:19:45 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 17:22:48 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_node(t_ast *node)
{
	ft_printf("====\nType : %d\n", node->node_type);
	printf("Cmd : %s\n", node->args[0]);
	print_redir(node->redir);
	print_strarray("Args", node->args);
}

void	print_strarray(char *name, char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	ft_printf("%s : [", name);
	while (arr[++i])
		ft_printf("\"%s\", ", arr[i - 1]);
	ft_printf("\"%s\"]\n", arr[i - 1]);
}

void	print_strarray_raw(char **arr, char sep)
{
	int	i;

	i = -1;
	if (!arr || !arr[0])
		return ;
	while (arr[++i] != NULL)
	{
		ft_putstr_fd(arr[i], STDOUT_FILENO);
		if (arr[i + 1] != NULL)
			ft_putchar_fd(sep, STDOUT_FILENO);
	}
}

void	print_strarray_endl(char *name, char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	ft_printf("%s\n", name);
	while (arr[++i])
		ft_printf("\"%s\"\n", arr[i - 1]);
	ft_printf("\"%s\"\n", arr[i - 1]);
}

void	print_env(char **env)
{
	int	i;

	i = -1;
	if (!env)
		return ;
	while (env[++i])
		ft_printf("%s\n", env[i]);
}
