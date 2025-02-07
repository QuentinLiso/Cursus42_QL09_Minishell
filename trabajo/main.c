#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;
	t_ast	*node;

	if (mnsh_initialization(ac, av, env, &mnsh))
		return (ERR_ARGS);
	
	char *str = "export BALLON=toto";
	char **tokens = ft_calloc(128, sizeof(char *));
	int	i = 0;
	ft_printf("Shell : %s\n", str);
	if (ft_strtok_mnsh(str, &tokens))
		return (ERR_QUOTE);
	// print_env(mnsh.env_mnsh);
	// char	*my_env;
	// my_env = ft_get_env_var(mnsh.env_mnsh, "PWD");
	// if (my_env)
	// 	printf("%s\n", my_env);
	int	tok_len = ft_strarrlen(tokens);
	// printf("%d\n", tok_len);
	print_strarray("Tokens", tokens);
	node = create_ast(tokens, 0, tok_len - 1);
	print_ast(node, 0);
	// mnsh_echo(tokens, &mnsh.last_exit_status);
	// mnsh_env(&mnsh);
	// mnsh_pwd(&mnsh);
	// mnsh_cd("", &mnsh);

	mnsh_env(&mnsh);
	// execute_ast(&node, &mnsh);
	// char *v = "";
	// printf("%d\n", export_check_specials(v));
	mnsh_export(tokens, &mnsh);
	char *unset_tok[] = {"BALLON", NULL};
	mnsh_unset(&mnsh.env_mnsh, unset_tok, &mnsh);
	mnsh_env(&mnsh);
	(void)i; (void)node;
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