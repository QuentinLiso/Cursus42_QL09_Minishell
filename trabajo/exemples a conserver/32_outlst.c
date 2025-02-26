#include "minishell.h"

t_outfiles	*outfiles_new(char *outfile, t_outstyle style)
{
	t_outfiles	*node;

	node = ft_calloc(1, sizeof(t_outfiles));
	if (!node)
		return (NULL);
	node->outfile = ft_strdup(outfile);
	node->outstyle = style;
	node->next = NULL;
	return (node);
}

void	outfiles_add_back(t_outfiles **outfiles, t_outfiles *new)
{
	t_outfiles	*iterator;

	if (!outfiles || !new)
		return ;
	if (!(*outfiles))
		*outfiles = new;
	else
	{
		iterator = outfiles_last(*outfiles);
		iterator->next = new;
	}
}

t_outfiles	*outfiles_last(t_outfiles *outfiles)
{
	if (!outfiles)
		return (NULL);
	while (outfiles->next != NULL)
	{
		outfiles = outfiles->next;
	}
	return (outfiles);
}

int	outfiles_size(t_outfiles *outfiles)
{
	int	len;

	if (!outfiles)
		return (0);
	len = 1;
	while (outfiles->next != NULL)
	{
		outfiles = outfiles->next;
		len++;
	}
	return (len);
}
