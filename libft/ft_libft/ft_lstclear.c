/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 22:55:16 by qliso             #+#    #+#             */
/*   Updated: 2024/11/30 10:04:07 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*iterator;

	if (!lst)
		return ;
	while (*lst != NULL)
	{
		iterator = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = iterator;
	}
}
