/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   30_ast_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:57:06 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:36 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_indir(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	t_token	*iterator;
	int		status;

	iterator = start;
	while (iterator != end->next)
	{
		if (iterator->type == TOKEN_INDIR)
		{
			status = is_indir_error(iterator, end);
			if (status)
				return (status);
			status = set_indir(node, iterator, mnsh);
			if (status)
				return (status);
			iterator = iterator->next;
		}
		iterator = iterator->next;
	}
	return (0);
}

int	is_indir_error(t_token *iterator, t_token *end)
{
	if (iterator == end || !iterator->next)
		return (perror2_mnsh(2, 3, ": ",
				"syntax error near unexpected token", " `newline'"));
	else if (iterator->next->type == TOKEN_INDIR)
		return (perror2_mnsh(2, 5, ": ", "syntax error near unexpected token",
				" `", iterator->word, "'"));
	else if (iterator->next->word && !*iterator->next->word)
		return (perror_mnsh(1, 2, "", "No such file or directory"));
	return (0);
}

int	set_indir(t_ast **node, t_token *iterator, t_mnsh *mnsh)
{
	if (!ft_strcmp(iterator->word, "<>"))
	{
		if (set_node_redir(node, iterator, REDIR_IN))
			return (ENOMEM);
		if (set_node_redir(node, iterator, REDIR_OUT))
			return (ENOMEM);
	}
	else if (!ft_strcmp(iterator->word, "<<"))
		return (set_node_heredoc(node, iterator, mnsh));
	else if (!ft_strcmp(iterator->word, "<"))
		return (set_node_redir(node, iterator, REDIR_IN));
	else if (!ft_strcmp(iterator->word, ">>"))
		return (set_node_redir(node, iterator, REDIR_APPEND));
	else if (!ft_strcmp(iterator->word, ">"))
		return (set_node_redir(node, iterator, REDIR_OUT));
	return (0);
}

int	set_node_redir(t_ast **node, t_token *iterator, t_redirstyle style)
{
	t_list		*redir_elem;
	t_redir		*redir_file;

	redir_elem = NULL;
	redir_file = ft_calloc(1, sizeof(t_redir));
	if (!redir_file)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set redir"));
	redir_file->file = ft_strdup(iterator->next->word);
	if (!redir_file->file)
	{
		free(redir_file);
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set redir"));
	}
	redir_file->style = style;
	redir_elem = ft_lstnew(redir_file);
	if (!redir_elem)
	{
		free_redir(redir_file);
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set redir"));
	}
	ft_lstadd_back(&(*node)->redir, redir_elem);
	return (0);
}

int	set_node_heredoc(t_ast **node, t_token *iterator, t_mnsh *mnsh)
{
	t_list	*redir_elem;
	t_redir	*redir_file;
	int		status;

	redir_elem = NULL;
	redir_file = ft_calloc(1, sizeof(t_redir));
	if (!redir_file)
		return (perror_mnsh(ENOMEM, 1, "malloc err in func set heredoc"));
	status = set_heredoc_name(&redir_file->file);
	if (status)
	{
		free(redir_file);
		return (perror_mnsh(1, 1, "could not set heredoc file"));
	}
	redir_file->style = REDIR_HEREDOC;
	redir_file->fd_heredoc_read = -1;
	redir_elem = ft_lstnew(redir_file);
	if (!redir_elem)
	{
		free_redir(redir_file);
		return (perror_mnsh(ENOMEM, 1, "malloc err in set heredoc"));
	}
	ft_lstadd_back(&(*node)->redir, redir_elem);
	return (create_heredoc(redir_file, iterator->next->word, mnsh));
}
