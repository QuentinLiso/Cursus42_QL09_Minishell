#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <stdbool.h>
# include <limits.h>

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/uio.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "libft.h"

# define BK "\033[30m"
# define R "\033[31m"
# define G "\033[32m"
# define Y "\033[33m"
# define B "\033[34m"
# define M "\033[35m"
# define C "\033[36m"
# define W "\033[37m"
# define RST "\033[0m"

# define MINISHELL B"=====================================\
======================================\n\n\
   __  __   ___   _   _   ___     \
   ___    _   _   _____   _       _       \n\
  |  \\/  | |_ _| | \\ | | |_ _|   \
  / ___|  | | | | | ____| | |     | |      \n\
  | |\\/| |  | |  |  \\| |  | |    \
  \\___ \\  | |_| | |  _|   | |     | |      \n\
  | |  | |  | |  | |\\  |  | |     \
  ___) | |  _  | | |___  | |___  | |___   \n\
  |_|  |_| |___| |_| \\_| |___|   \
  |____/  |_| |_| |_____| |_____| |_____|  \n\n\n\
========================================\
===================================\n\n"RST

# define MAX_TOKENS 128
# define MAX_TOKEN_LEN 128
# define OPERATORS "&& || >> << | < > & ;"
# define SPECIALS "&|><;\"'"

# define ERR_NOERR_MSG "no error\n"
# define ERR_ARGS_MSG "args error\n"
# define ERR_ENV_MSG "env error\n"
# define ERR_QUOTE_MSG "quote error\n"

typedef struct sigaction	t_sa;

typedef enum	e_error
{
	CONTINUE = -1,
	ERR_NOERR,
	ERR_ARGS,
	ERR_MALLOC,
	ERR_ENV,
	ERR_QUOTE
}	t_error;

typedef enum	e_nodetype
{
	NODE_CMD,
	NODE_OP
}	t_nodetype;

typedef struct minishell
{
	t_error	status;
	char	**env_mnsh;
	char	*prompt;
	t_sa	sa;
}	t_mnsh;

// Ast stands for Abstract Syntax Tree
typedef struct ast
{
	t_nodetype	type;
	char		*value;
	char		**args;
	struct ast	*left_node;
	struct ast	*right_node;
}	t_ast;


t_error	mnsh_initialization(int ac, char **av, char **env, t_mnsh *mnsh);
void	init_mnsh_struct(t_mnsh *mnsh);
t_error	duplicate_env(char **env, char ***env_mnsh);
void	init_sigaction(t_sa *sa, void (*action)(int, siginfo_t *, void *));
void	handle_signal(int signum, siginfo_t *info, void *other);
void	print_minishell_header();
int		mnsh_prompt(char **buf);

// tokens
t_error	ft_strtok_mnsh(char *s, char ***tokens);
t_error	tok_check(char **s, char ***tokens, int *i);
bool	tok_check_spaces(char **s);
t_error	tok_check_operator(char **s, char ***tokens, int *i);
t_error	tok_check_quotes(char **s, char ***tokens, int *i);
t_error	tok_check_regular(char **s, char ***tokens, int *i);

// ast
t_ast	*create_ast(char **tokens, int start, int end);
int		set_split_index(char **tokens, int start, int end);
int		set_args_count(char **tokens, int start, int end);
t_ast	*create_ast_opnode(char *op);
t_ast	*create_ast_cmdnode(char *cmd, char **args, int arg_count);
int		get_operator_precedence(char *op);
void	free_ast(t_ast *root_node);

// helpers
void	print_node(t_ast *node);
void	print_ast(t_ast *node, int depth);

// Utils
t_error	mnsh_perror(t_error	error);
t_error	perror_malloc(char *func_name);
void	print_strarray(char **arr);
void	free_strarray(char ***arr);
bool	ft_isspace(char c);
bool	ft_isspecial(char c);
int		is_operator(const char *s);
char	*ft_strndup(const char *s, int n);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strarrlen(char **arr);

#endif
