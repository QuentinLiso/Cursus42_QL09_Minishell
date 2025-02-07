#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;

	if (mnsh_initialization(ac, av, env, &mnsh))
		return (ERR_ARGS);

	char *str = "cd .. && ls";
	ft_printf("Shell : %s\n", str);

	if (ft_strtok_mnsh(str, &mnsh.tokens))
		return (ERR_QUOTE);
	print_strarray("Tokens", mnsh.tokens);

	mnsh.node = create_ast(mnsh.tokens, 0, ft_strarrlen(mnsh.tokens) - 1);
	print_ast(mnsh.node, 0);
	execute_ast(&mnsh.node, &mnsh);
	// print_strarray_raw(mnsh.env_mnsh, '\n');
	// char	*exp_tok[] = {"export", NULL};
	// mnsh_export(exp_tok, &mnsh);
	// ft_free_strarray(&mnsh.tokens);
	// ft_free_strarray(&mnsh.env_mnsh);
	// ft_free_strarray(&mnsh.paths);
	// print_minishell_header();
	
	// while (1)
	// {
	// 	if (mnsh_prompt(&(mnsh.prompt)))
	// 		return (EXIT_FAILURE);
	// 	ft_printf("%s\n", mnsh.prompt);
	// 	if (ft_strncmp(mnsh.prompt, "exit", 4) == 0)
	// 		return (EXIT_SUCCESS);
	// }
	return (EXIT_SUCCESS);
}

int		mnsh_prompt(char **buf)
{
	char	*cwd;
	
	cwd = getcwd(NULL, 0);
	ft_printf(R "%s 🐚 : " RST, cwd);
	*buf = readline("$> ");
	return (0);	
}

//============ TESTS à copier coller dans main =======

	// mnsh_echo(mnsh.tokens, &mnsh);
	// mnsh_env(&mnsh);
	// mnsh_pwd(&mnsh);
	// mnsh_cd(mnsh.tokens, &mnsh);
	// mnsh_pwd(&mnsh);
	// printf(B "\n*******************************************"RST);
	// mnsh_export(mnsh.tokens, &mnsh);
	// printf(R "\n*******************************************"RST);
	// char	*exp_tok[] = {"export", "BALLON=salutcoucou", NULL};
	// mnsh_export(exp_tok, &mnsh);
	// mnsh_export(mnsh.tokens, &mnsh);
	// printf(G "\n*******************************************"RST);
	// char *unset_tok[] = {"unset", "BALLON", "HOME", "ZSH", NULL};
	// mnsh_unset(unset_tok, &mnsh, &mnsh.env_mnsh);
	// mnsh_export(mnsh.tokens, &mnsh);
	// mnsh_exit(mnsh.tokens, &mnsh);