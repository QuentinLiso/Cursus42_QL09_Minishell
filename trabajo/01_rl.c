#include "minishell.h"

int		mnsh_prompt(char **prompt)
{
	char	*minishell;

	minishell = ft_strjoin_multi(3, B, "🐚 MINISHELL 🐚 > ", RST);
	*prompt = readline(minishell);
	ft_free_str(&minishell);
	if (!prompt)
		return (-1);
	else if (!*prompt)
		return (1);
	return (0);	
}