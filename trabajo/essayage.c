#include "minishell.h"

void	free_lst_str(void *ptr)
{
	char	*str;

	str = (char *)ptr;
	if (str)
		ft_free_str(&str);
}

void	del_node(t_list **list, char *str)
{
	t_list	*iter;
	t_list	*prev;

	if (!list || !*list || !str)
		return ;
	iter = *list;
	prev = NULL;
	while (iter && ft_strcmp((char *)iter->content, str))
	{
		prev = iter;
		iter = iter->next;
	}
	if (!iter)
		return ;
	if (!prev)
	{
		*list = iter->next;
		ft_lstdelone(iter, &free_lst_str);
		return ;
	}
	prev->next = iter->next;
	ft_lstdelone(iter, &free_lst_str);
}

int	main()
{
	t_list	*node1;
	t_list	*node2;
	t_list	*node3;
	t_list	*node4;

	node1 = ft_lstnew(ft_strdup("bonjour"));
	node2 = ft_lstnew(ft_strdup("tout"));
	node3 = ft_lstnew(ft_strdup("le monde"));
	node4 = ft_lstnew(ft_strdup("les copains"));
	ft_lstadd_back(&node1, node2);
	ft_lstadd_back(&node1, node3);
	ft_lstadd_back(&node1, node4);
	print_infiles(node1);
	del_node(&node1, "bonjour");
	del_node(&node1, "le monde");
	print_infiles(node1);
	return (0);
}