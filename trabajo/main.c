#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;

	char	*str = "echo $blabla>>'$D'$|cat -e && ls -l";
	if (mnsh_initialization(ac, av, env, &mnsh))
		return (ERR_ARGS);
	mnsh.prompt = ft_strdup(str);
	mnsh.tokis = ft_strtok_mnsh(str, &mnsh);
	print_tokis(mnsh.tokis);
	printf("==================================\n");
	expand_tok_mnsh(&mnsh.tokis, &mnsh);
	print_tokis(mnsh.tokis);
	printf("==================================\n");
	// printf("%d\n", final_args_count(mnsh.tokis, mnsh.last_tokis));
	// expand_tok_mnsh(&mnsh.tokens);
	// print_strarray("Tokens", mnsh.tokens);
	mnsh.node = create_ast(mnsh.tokis, mnsh.last_tokis);
	print_ast(mnsh.node, 0);
	// execute_ast(&mnsh.node, &mnsh);

	// while (1)
	// {
	// 	ft_free_str(&mnsh.prompt);
	// 	if (mnsh_prompt(&mnsh.prompt))
	// 		continue;
	// 	if (ft_strtok_mnsh(mnsh.prompt, &mnsh.tokens))
	// 		continue;
	// 	print_strarray("Tokens", mnsh.tokens);
	// 	mnsh.node = create_ast(mnsh.tokens, 0, ft_strarrlen(mnsh.tokens) - 1);
	// 	print_ast(mnsh.node, 0);
	// 	execute_ast(&mnsh.node, &mnsh);
	// }
	ft_free_str(&mnsh.prompt);
	// ft_free_strarray(&mnsh.tokens);
	ft_free_strarray(&mnsh.env_mnsh);
	ft_free_strarray(&mnsh.paths);
	return (EXIT_SUCCESS);
}


//============ TESTS à copier coller dans main =======

	// char *str = "cd .. && ls -l";
	// ft_printf("Shell : %s\n", str);

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