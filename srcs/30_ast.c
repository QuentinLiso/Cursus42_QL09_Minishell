/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   30_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:55:31 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/05 10:03:26 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ast_mnsh(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	int	status;

	if (!start || !end)
		return (0);
	status = create_ast(node, start, end, mnsh);
	if (status)
	{
		mnsh->last_exit_status = status;
		return (status);
	}
	return (0);
}

int	create_ast(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh)
{
	t_token	*split_token;

	if (start == end->next)
		return (0);
	split_token = set_split_token(start, end);
	if (split_token == NULL)
		return (ast_cmdnode(node, start, end, mnsh));
	else if (is_syntax_error(start, end, split_token))
		return (errno_to_exit(2));
	return (ast_opnode(node, start, end, mnsh));
}

t_token	*set_split_token(t_token *start, t_token *end)
{
	t_token	*iterator;
	t_token	*split_token;
	int		precedence;
	int		lowest_precedence;

	split_token = NULL;
	lowest_precedence = INT_MAX;
	iterator = start;
	while (iterator != end->next)
	{
		precedence = get_operator_precedence(iterator->word);
		if (precedence > 0 && precedence <= lowest_precedence)
		{
			split_token = iterator;
			lowest_precedence = precedence;
		}
		iterator = iterator->next;
	}
	return (split_token);
}

int	get_operator_precedence(char *op)
{
	if (!op)
		return (0);
	else if (!ft_strcmp(op, "||"))
		return (1);
	else if (!ft_strcmp(op, "&&"))
		return (2);
	else if (!ft_strcmp(op, "|"))
		return (3);
	return (0);
}

bool	is_syntax_error(t_token *start, t_token *end, t_token *split_token)
{
	if (split_token == start || split_token == end
		|| split_token->next == NULL
		|| split_token->next->type == TOKEN_OPERATOR
		|| split_token->prev == NULL
		|| split_token->prev->type == TOKEN_OPERATOR)
	{
		perror2_mnsh(2, 4, ": ", "syntax error near unexpected token `",
			split_token->word, "'");
		return (true);
	}
	else
	{
		return (false);
	}
}
