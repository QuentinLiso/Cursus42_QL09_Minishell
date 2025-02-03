#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_mnsh	mnsh;
	t_ast	*node;

	if (mnsh_initialization(ac, av, env, &mnsh))
		return (ERR_ARGS);
	
	char *str = ">> tuto.out /bin/ls -l | /bin/grep mini ";// > tutu.out >> totu.out";
	char **tokens = ft_calloc(128, sizeof(char *));
	int	i = 0;
	printf("Shell : %s\n", str);
	if (ft_strtok_mnsh(str, &tokens))
		return (ERR_QUOTE);
	int	tok_len = ft_strarrlen(tokens);
	printf("%d\n", tok_len);
	print_strarray("Tokens", tokens);
	node = create_ast(tokens, 0, tok_len - 1);
	print_ast(node, 0);
	
	execute_ast(&node, &mnsh);
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

void	execute_ast(t_ast **node, t_mnsh *mnsh)
{
	if (!(*node))
		return ;
	if ((*node)->node_type == NODE_CMD)
		exec_ast_cmd(node, mnsh);
	else if ((*node)->node_type == NODE_OP)
		exec_ast_op(node, (*node)->op_type, mnsh);
}

void	exec_ast_op(t_ast **node, t_optype op, t_mnsh *mnsh)
{
	if (op == OP_AND)
		exec_ast_op_and(node, mnsh);
	else if (op == OP_OR)
		exec_ast_op_or(node, mnsh);
	else if (op == OP_PIPE)
		exec_ast_op_pipe(node, mnsh);
}

int		mnsh_prompt(char **buf)
{
	char	*cwd;
	
	cwd = getcwd(NULL, 0);
	ft_printf(R "%s 🐚 : " RST, cwd);
	*buf = readline("$> ");
	return (0);	
}