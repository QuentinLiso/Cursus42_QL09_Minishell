#include "minishell.h"

void	ft_free_str(char **ptr)
{
	if (*ptr == NULL)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void	safe_free_str(void *ptr)
{
	char	*str;

	str = (char *)ptr;
	if (str == NULL)
		return ;
	free(str);
	str = NULL;
}

void	ft_free_strarray(char ***arr)
{
	int		i;

	i = -1;
	if ((*arr) == NULL)
		return ;
	while ((*arr)[++i])
		ft_free_str(&(*arr)[i]);
	free(*arr);
}

t_error		ft_free_strarray_perror(char ***arr, t_error err)
{
	ft_free_strarray(arr);
	return (err);
}

void	ft_free_all_tok(t_token **tok)
{
	t_token	*iterator;

	if (!tok && !*tok)
		return ;
	while (*tok != NULL)
	{
		iterator = (*tok)->next;
		ft_free_str(&(*tok)->word);
		free((*tok));
		*tok = iterator;
	}
}

void	ft_free_infiles(t_infiles *infiles)
{
	t_infiles	*iterator;
	t_infiles	*buffer;

	if (!infiles)
		return ;
	iterator = infiles;
	while (iterator)
	{
		buffer = iterator;
		iterator = iterator->next;
		if (buffer && buffer->infile)
		{
			ft_free_str(&buffer->infile);
			free(buffer);
		}
	}
}

void	free_outfile(void *ptr)
{
	t_outfile	*outfile;

	outfile = (t_outfile *)ptr;
	if (outfile)
	{
		ft_free_str(&outfile->file);
		free(outfile);
	}
	outfile = NULL;
}

void	ft_free_ast(t_ast *root_node)
{
	if (!root_node)
		return ;
	ft_free_ast(root_node->left_node);
	ft_free_ast(root_node->right_node);
	ft_lstclear(&root_node->infiles, &safe_free_str);
	ft_lstclear(&root_node->heredocs, &safe_free_str);
	ft_lstclear(&root_node->outfiles, &free_outfile);
	ft_free_strarray(&root_node->args);
	free(root_node);
}

int		free_ast_ret(t_ast *root_node, int errnum)
{
	ft_free_ast(root_node);
	return (errnum);
}

void	ft_free_reset_mnsh(t_mnsh *mnsh)
{
	ft_free_str(&mnsh->prompt);
	ft_free_all_tok(&mnsh->tokis);
	ft_free_ast(mnsh->node);
}

void	ft_free_all_mnsh(t_mnsh *mnsh)
{
	rl_clear_history();
	ft_free_str(&mnsh->prompt);
	ft_free_all_tok(&mnsh->tokis);
	ft_free_strarray(&mnsh->paths);
}