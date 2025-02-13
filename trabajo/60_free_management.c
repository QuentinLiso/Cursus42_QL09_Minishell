#include "minishell.h"

void	ft_free_str(char **ptr)
{
	if (*ptr == NULL)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void		ft_free_strarray(char ***arr)
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

	if (!tok)
		return ;
	while (*tok != NULL)
	{
		iterator = (*tok)->next;
		if ((*tok)->word)
			free((*tok)->word);
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

void	ft_free_outfiles(t_outfiles *outfiles)
{
	t_outfiles	*iterator;
	t_outfiles	*buffer;

	if (!outfiles)
		return ;
	iterator = outfiles;
	while (iterator)
	{
		buffer = iterator;
		iterator = iterator->next;
		if (buffer && buffer->outfile)
		{
			ft_free_str(&buffer->outfile);
			free(buffer);
		}
	}
}

void	ft_free_ast(t_ast *root_node)
{
	if (!root_node)
		return ;
	ft_free_ast(root_node->left_node);
	ft_free_ast(root_node->right_node);
	ft_free_infiles(root_node->infiles);
	ft_free_outfiles(root_node->outfiles);
	ft_free_strarray(&root_node->heredoc);
	ft_free_strarray(&root_node->args);
	free(root_node);
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
	ft_free_strarray(&mnsh->env_mnsh);
	ft_free_strarray(&mnsh->paths);
}