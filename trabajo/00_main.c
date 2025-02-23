#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;
	char	*str[] = {
		"echo aaa && echo bbb | cat > out -dde -f -g",
		NULL};
	int		i = -1;
	if (mnsh_initialization(&mnsh, ac, env))
	{
		ft_free_all_mnsh(&mnsh);
		return (mnsh.last_exit_status);
	}
	// ft_lstiter(mnsh.env_mnsh_lst, &display_env_var);
	// print_strarray_endl("PATHS", mnsh.paths);
	while (str[++i])
	{
		ft_free_reset_mnsh(&mnsh);
		printf("=======================\n");
		printf("STR : %s\n", str[i]);
		mnsh.prompt = ft_strdup(str[i]);
		if (strtok_mnsh(&mnsh, mnsh.prompt) < 0)
		{
			printf("EXIT STATUS : %d\n", mnsh.last_exit_status);
			ft_free_all_mnsh(&mnsh);
			return (mnsh.last_exit_status);
		}
		print_tokis(mnsh.tokis);
		printf("=======================\n");
		create_ast_mnsh(&mnsh, &mnsh.node, mnsh.tokis, mnsh.last_tokis);
		print_ast(mnsh.node, 0);
	}
	// ft_free_reset_mnsh(&mnsh);
	// ft_lstclear(&mnsh.env_mnsh_lst, &free_env_var);
	// ft_free_strarray(&mnsh.paths);

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
	// if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	// 	print_minishell_header();
	// while (1)
	// {
	// 	ft_free_reset_mnsh(&mnsh);
	// 	status = mnsh_prompt(&mnsh.prompt);
	// 	if (status == 1)
	// 		exit (0);
	// 	add_history(mnsh.prompt);
	// 	mnsh.tokis = ft_strtok_mnsh(mnsh.prompt, &mnsh);
	// 	if (mnsh.tokis == NULL)
	// 		continue;
	// 	mnsh.node = create_ast(mnsh.tokis, mnsh.last_tokis, &mnsh);
	// 	execute_ast(&mnsh.node, &mnsh);
	// }
	// ft_free_all_mnsh(&mnsh);
	(void)ac; (void)av; (void)env;
	return (EXIT_SUCCESS);
}