/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   60_free_management_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:24:17 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 16:24:50 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	free_tokis(t_token **tok)
{
	t_token	*iterator;

	if (!tok && !*tok)
		return ;
	while (*tok != NULL)
	{
		iterator = (*tok)->next;
		free_str(&(*tok)->word);
		free((*tok));
		*tok = iterator;
	}
}

void	free_redir(void *ptr)
{
	t_redir		*redir_file;

	redir_file = (t_redir *)ptr;
	if (redir_file)
	{
		free_str(&redir_file->file);
		redir_file->fd_heredoc_read = -1;
		free(redir_file);
	}
	redir_file = NULL;
}

void	free_ast_node(t_ast **node)
{
	if (!*node)
		return ;
	free_ast_node(&(*node)->left_node);
	free_ast_node(&(*node)->right_node);
	free_strarray(&(*node)->args);
	ft_lstclear(&(*node)->redir, &free_redir);
	free(*node);
	*node = NULL;
}

void	free_reset_mnsh(t_mnsh *mnsh)
{
	if (!mnsh)
		return ;
	free_str(&mnsh->prompt);
	free_tokis(&mnsh->tokis);
	free_ast_node(&mnsh->node);
}

void	free_all_mnsh(t_mnsh *mnsh)
{
	rl_clear_history();
	free_str(&mnsh->prompt);
	free_tokis(&mnsh->tokis);
	free_ast_node(&mnsh->node);
	ft_lstclear(&mnsh->env_mnsh_lst, &free_env_var);
}
