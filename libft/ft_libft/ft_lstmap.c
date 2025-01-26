/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 22:56:46 by qliso             #+#    #+#             */
/*   Updated: 2024/11/11 22:56:50 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*iterator_old;
	t_list	*iterator_new;

	if (!lst)
		return (NULL);
	new_lst = ft_lstnew((*f)(lst->content));
	if (!new_lst)
		return (NULL);
	iterator_old = lst->next;
	iterator_new = new_lst;
	while (iterator_old)
	{
		iterator_new->next = ft_lstnew((*f)(iterator_old->content));
		if (!(iterator_new->next))
		{
			ft_lstclear(&lst, del);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		iterator_old = iterator_old->next;
		iterator_new = iterator_new->next;
	}
	return (new_lst);
}
