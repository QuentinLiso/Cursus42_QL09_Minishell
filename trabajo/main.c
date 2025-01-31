#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;
	t_ast	*node;

	if (mnsh_initialization(ac, av, env, &mnsh))
		return (ERR_ARGS);
	
	char *str = "ls -l | grep \"info\" | wc -l || testminishell && echo bite << heredoc >> outfile.txt";
	char **tokens = ft_calloc(128, sizeof(char *));
	int	i = 0;
	if (ft_strtok_mnsh(str, &tokens))
		return (ERR_QUOTE);
	int	tok_len = ft_strarrlen(tokens);
	print_strarray(tokens);
	ft_printf("%s\n", str);
	node = create_ast(tokens, 0, tok_len - 1);
	print_ast(node, 0);

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