/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 22:54:03 by qliso             #+#    #+#             */
/*   Updated: 2024/11/30 10:42:25 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*iterator;

	if (!lst || !new)
		return ;
	if (!(*lst))
		*lst = new;
	else
	{
		iterator = ft_lstlast(*lst);
		iterator->next = new;
	}
}
