#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;
	int		status;

	if (mnsh_initialization(ac, av, env, &mnsh))
		return (ERR_ARGS);
/*
	char	*str[] = {
		"echo aaa && echo bbbb", 
		"$ZYZ",
		"| hola",
		"| | |",
		"||",
		"|||||",
		"|||||||||||",
		">>|<<",
		"&&",
		"&&&&&",
		"&&&&&&&&&&",
		";;",
		NULL};
	int		i = -1;
	while (str[++i])
	{
		ft_free_reset_mnsh(&mnsh);
		// printf("=======================\n");
		mnsh.prompt = ft_strdup(str[i]);
		mnsh.tokis = ft_strtok_mnsh(mnsh.prompt, &mnsh);
		// print_tokis(mnsh.tokis);
		mnsh.node = create_ast(mnsh.tokis, mnsh.last_tokis);
		// print_ast(mnsh.node, 0);
		execute_ast(&mnsh.node, &mnsh);
	}
	ft_free_all_mnsh(&mnsh);
*/
	while (1)
	{
		ft_free_reset_mnsh(&mnsh);
		status = mnsh_prompt(&mnsh.prompt);
		if (status == 1)
			exit (0);
		add_history(mnsh.prompt);
		mnsh.tokis = ft_strtok_mnsh(mnsh.prompt, &mnsh);
		if (mnsh.tokis == NULL)
			continue;
		mnsh.node = create_ast(mnsh.tokis, mnsh.last_tokis);
		execute_ast(&mnsh.node, &mnsh);
	}
	ft_free_all_mnsh(&mnsh);
	return (EXIT_SUCCESS);
}