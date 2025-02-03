#include "minishell.h"

t_infiles	*infiles_new(char *infile)
{
	t_infiles	*node;

	node = ft_calloc(1, sizeof(t_infiles));
	if (!node)
		return (NULL);
	node->infile = ft_strdup(infile);
	node->next = NULL;
	return (node);
}

void	infiles_add_back(t_infiles **infiles, t_infiles *new)
{
	t_infiles	*iterator;

	if (!infiles || !new)
		return ;
	if (!(*infiles))
		*infiles = new;
	else
	{
		iterator = infiles_last(*infiles);
		iterator->next = new;
	}
}

t_infiles	*infiles_last(t_infiles *infiles)
{
	if (!infiles)
		return (NULL);
	while (infiles->next != NULL)
	{
		infiles = infiles->next;
	}
	return (infiles);
}

int	infiles_size(t_infiles *infiles)
{
	int	len;

	if (!infiles)
		return (0);
	len = 1;
	while (infiles->next != NULL)
	{
		infiles = infiles->next;
		len++;
	}
	return (len);
}
