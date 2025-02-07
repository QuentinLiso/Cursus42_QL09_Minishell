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
# include <sys/stat.h>

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
# define AST_OPERATORS "&& || | & ;"
# define TOK_OPERATORS "&& || | & ; << < >> >"
# define AST_SPECIALS "&|;\"'"
# define TOK_SPECIALS "&|;<>\"'"
# define INDIR ">> > << <"

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
	ERR_QUOTE,
	ERR_CD,
	ERR_EXPORT
}	t_error;

typedef enum	e_nodetype
{
	NODE_CMD,
	NODE_OP
}	t_nodetype;

typedef enum	e_optype
{
	OP_NULL,
	OP_AND,
	OP_OR,
	OP_PIPE,
	OP_OUT,
	OP_OUTAPD,
	OP_IN,
	OP_HEREDOC
}	t_optype;

typedef enum	e_outstyle
{
	OUT_NULL,
	OUT_TRUNC,
	OUT_APPEND
}	t_outstyle;

typedef enum	e_instyle
{
	IN_NULL,
	IN_FILE,
	IN_HEREDOC
}	t_instyle;

typedef struct s_infiles
{
	char				*infile;
	struct s_infiles	*next;
}	t_infiles;

typedef struct s_outfiles
{
	char				*outfile;
	t_outstyle			outstyle;
	struct s_outfiles	*next;
}	t_outfiles;

// Ast stands for Abstract Syntax Tree
typedef struct s_ast
{
	t_nodetype	node_type;
	t_optype	op_type;
	char		**args;
	t_infiles	*infiles;
	char		**heredoc;
	char		*heredoc_end;
	t_instyle	instyle;
	t_outfiles		*outfiles;
	struct s_ast	*left_node;
	struct s_ast	*right_node;
}	t_ast;

typedef struct s_minishell
{
	t_error	status;
	char	**env_mnsh;
	char	**paths;
	char	**tokens;
	char	*prompt;
	int		last_exit_status;
	t_sa	sa;
	t_ast	*node;
}	t_mnsh;



// init
t_error	mnsh_initialization(int ac, char **av, char **env, t_mnsh *mnsh);
void	init_mnsh_struct(t_mnsh *mnsh);
t_error	duplicate_env(char **env_src, int len, char ***env_dst);
t_error	set_mnsh_paths(char **env_mnsh, char ***paths);
char	*ft_get_env_var(char **env, char *var);
int		ft_get_env_var_index(char **env, char *var);
t_error	ft_reset_env_var(char ***env, char *var, char *value);
t_error	ft_reset_env_var_index(char ***env, char *var, char *value, int i);
t_error	ft_new_env_var(char ***env, char *var, char *value);
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
int		get_operator_precedence(char *op);
t_ast	*create_ast_opnode(char *op);
t_optype	set_op_type(char *op);
t_ast	*create_ast_cmdnode(char ***cmd_tok, int end);
t_ast   *init_cmd_node(char ***cmd_tok, int end);
int     final_args_count(char **cmd_tok, int end);
void    set_node_outfile(char ***tok, t_ast **node, int *i, t_outstyle style);
void    set_node_infile(char ***tok, t_ast **node, int *i);
void    set_node_heredoc(char ***cmd_tok, t_ast **node, int *i);
void    lst_to_arr(t_list *heredoc, t_ast **node);
void	free_ast(t_ast *root_node);

// exec
void	expand_env_vars(char ***args, t_mnsh *mnsh);
t_error strtok_arg_env(char *s, char ***tokens, t_mnsh *mnsh);
t_error env_check(char **s, char ***tokens, int *i, t_mnsh *mnsh);
t_error env_check_dollar(char **s, char ***tokens, int *i, t_mnsh *mnsh);
char    *expand_env_var(char *start, char *s, t_mnsh *mnsh);
t_error env_check_regular(char **s, char ***tokens, int *i);
char    *ft_strcat_arr(char **arr);

void	set_cmd_path(char **cmd, char **paths);

void	execute_ast(t_ast **node, t_mnsh *mnsh);
void	exec_ast_cmd(t_ast **node, t_mnsh *mnsh);
void	exec_ast_cmd_in(t_ast **node, int *fd);
void	exec_ast_cmd_infile(char *last_infile, int *fd);
char	*check_infiles(t_ast **node);
void	exec_ast_cmd_heredoc(t_ast **node);
void	exec_ast_cmd_out(t_ast **node, int *fd);
void	exec_ast_cmd_outfile(char *outfile, int *fd, int flag);

void	exec_ast_op(t_ast **node, t_optype op, t_mnsh *mnsh);
void	exec_ast_op_and(t_ast **node, t_mnsh *mnsh);
void	exec_ast_op_or(t_ast **node, t_mnsh *mnsh);
void	exec_ast_op_pipe(t_ast **node, t_mnsh *mnsh);
void	left_pipe(t_ast **node, int (*fd)[2], t_mnsh *mnsh);
void	right_pipe(t_ast **node, int (*fd)[2], t_mnsh *mnsh);

// builtins
bool	is_builtin(char *s);
t_error	b_in(char *s, char **args, t_mnsh *mnsh, char ***env);
t_error	mnsh_echo(char **args, t_mnsh *mnsh);
t_error	mnsh_env(t_mnsh *mnsh);
t_error	mnsh_pwd(t_mnsh *mnsh);
t_error	mnsh_cd(char **args, t_mnsh *mnsh);
t_error	mnsh_export(char **args, t_mnsh *mnsh);
t_error	ft_print_export_var(char **env);
void	ft_print_substr_before_char(char *s, char c, int *j);
void	ft_print_substr_after_char(char *s, int *j);
t_error	export_check_specials(char *s, int *j);
t_error	handle_export_var(char ***env, char *arg, int *j);
t_error	mnsh_unset(char **args, t_mnsh *mnsh, char ***env);
size_t	unset_newenvlen(char ***env, char **args);
bool	env_var_is_in_args(char *var, char **args);
bool	str_is_in_arr(char *s, char **arr);
t_error	mnsh_exit(char **args, t_mnsh *mnsh);

// helpers
void	print_node(t_ast *node);
void	print_ast(t_ast *node, int depth);
void	print_strarray(char *name, char **arr);
void	print_strarray_raw(char **arr, char sep);
void	print_strarray_endl(char *name, char **arr);
void	print_env(char **env);
void	print_infiles(t_infiles *infiles);
void	print_outfiles(t_outfiles *outfiles);

// Utils
t_error	mnsh_perror(t_error	error);
t_error	perror_malloc(char *func_name);
void	ft_free_str(char **ptr);
void	ft_free_strarray(char ***arr);
t_error		ft_free_strarray_perror(char ***arr, t_error err);
bool	ft_isspace(char c);
bool	ft_isspecial(char c, const char *list_specials);
bool	ft_str_contain(char *s, char c);
int		is_operator(const char *s, const char *list_operators);
int		is_indir(const char *s);
size_t	ft_strlenchar(char *s, char c);
char	*ft_strndup(const char *s, int n);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strarrlen(char **arr);
char	*ft_strjoin_multi(int count, ...);
size_t  ft_strjoin_multi_getlen(int count, va_list args);
char    *ft_strjoin_multi_setstr(size_t len, int count, va_list args);

// outfiles lst
t_outfiles	*outfiles_new(char *outfile, t_outstyle style);
void	outfiles_add_back(t_outfiles **outfiles, t_outfiles *new);
t_outfiles	*outfiles_last(t_outfiles *outfiles);
int	outfiles_size(t_outfiles *outfiles);

// infiles lst
t_infiles	*infiles_new(char *infile);
void	infiles_add_back(t_infiles **infiles, t_infiles *new);
t_infiles	*infiles_last(t_infiles *infiles);
int	infiles_size(t_infiles *infiles);

#endif
