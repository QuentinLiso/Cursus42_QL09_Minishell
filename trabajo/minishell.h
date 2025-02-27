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
# include <termios.h>

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

# define MINISHELL_PROMPT B"🐚 Minishell 🐚 > "RST
// # define MINISHELL_PROMPT "Minishell > "
# define TOK_OPERATORS "&& || | &"
# define TOK_INDIR "<> >> > << <"
# define TOK_SPECIALS "&|<>"
# define ENV_SPECIALS "#*@$!0123456789-"

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

typedef enum	e_toktype
{
	TOKEN_NOTYPE,
	TOKEN_WORD,
	TOKEN_INDIR,
	TOKEN_OPERATOR
}	t_toktype;

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
	OP_PIPE
}	t_optype;

typedef enum	e_instyle
{
	IN_NULL,
	IN_FILE,
	IN_HEREDOC
}	t_instyle;

typedef enum	e_outstyle
{
	OUT_NULL,
	OUT_TRUNC,
	OUT_APPEND
}	t_outstyle;

typedef struct	s_var
{
	char			*key;
	char			*value;
}	t_var;

typedef struct s_token
{
	char			*word;
	t_toktype		type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_outfile
{
	char			*file;
	t_outstyle		outstyle;
}	t_outfile;

typedef struct s_ast
{
	t_nodetype	node_type;
	t_optype	op_type;
	char		**args;
	t_list		*infiles;
	t_list		*heredocs;
	char		**heredoc;
	char		*heredoc_end;
	t_instyle	instyle;
	t_list		*outfiles;
	struct s_ast	*left_node;
	struct s_ast	*right_node;
}	t_ast;

typedef struct s_minishell
{
	t_list	*env_mnsh_lst;
	char	**paths;
	t_token	*tokis;
	t_token	*last_tokis;
	char	*prompt;
	int		last_exit_status;
	char	*last_cmd_arg;
	t_sa	sa;
	t_ast	*node;
}	t_mnsh;


// readline
void	print_minishell_header();
int		mnsh_prompt(char **prompt);

// signals
void	init_sigaction(t_sa *sa, void (*action)(int, siginfo_t *, void *));
void	handle_signal(int signum, siginfo_t *info, void *other);

// init
int		mnsh_initialization(t_mnsh *mnsh, int ac, char **env);
void	init_mnsh_struct(t_mnsh *mnsh);
int		set_mnsh_env(t_mnsh *mnsh, t_list **env_lst, char **env);
int		set_mnsh_empty_env(t_list **env_lst);
int		set_mnsh_env_shlvl(t_list **env_lst);
int		set_mnsh_paths(t_list *env_mnsh_list, char ***paths);

// env
t_var	*alloc_env_var(char *key_eq_val);
int		free_env_var_ret(t_var *var, int ret);
void	free_env_var(void *ptr);
char	*get_env_var(t_list *env_lst, char *key);
t_list	*get_env_var_prev(t_list *env_lst, char *key);
int		add_env_var(t_list **env_lst, char *key, char *value);
int		edit_env_var(t_list **env_lst, char *key, char *value);
void	display_env_var(void *ptr);


// tokens
t_token	*new_toki(char *word, t_toktype type);
int		add_toki_mnsh(t_mnsh *mnsh, char *word, t_toktype t);
int		strtok_mnsh(t_mnsh *mnsh, char *s);
int		tok_check(t_mnsh *mnsh, char **s);
int		tok_check_spaces(char **s);
int		tok_check_ope_ind(t_mnsh *mnsh, char **s, char *spec, t_toktype type);
int		is_operator(const char *s, const char *list_operators);
int		tok_check_regular(t_mnsh *mnsh, char **s);
int		tok_check_regular_split(t_mnsh *mnsh, char **s, char **word, bool *q);
int		split_quote(char **s, char **word);
int		split_dquote(t_mnsh *mnsh, char **s, char **word);
int		check_split_dquote(t_mnsh *mnsh, char **s, char **buffer);
int		split_dquote_env(t_mnsh *mnsh, char **s, char **buffer);
int		split_dquote_env_spec(t_mnsh *mnsh, char **s, char **buffer);
int		get_last_exit(t_mnsh *mnsh, char **s, char **buffer);
int		split_dquote_noenv(char **s, char **buffer);
void	append_dquote(char **word, char **buffer);
int		split_noquote(t_mnsh *mnsh, char **s, char **word);
int		check_split_noquote(t_mnsh *mnsh, char **s, char **buffer);
int		split_noquote_env(t_mnsh *mnsh, char **s, char **buffer);
int		split_noquote_env_spec(t_mnsh *mnsh, char **s, char **buffer);
int		split_noquote_noenv(char **s, char **buffer);

// ast
int			ast_mnsh(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh);
int			create_ast(t_ast **node, t_token *start, t_token *end);
t_token		*set_split_token(t_token *start, t_token *end);
int			get_operator_precedence(char *op);
bool		is_syntax_error(t_token *start, t_token *end, t_token *split_token);
int			ast_opnode(t_ast **node, t_token *start, t_token *split_tok, t_token *end);
int 		create_ast_opnode(t_ast **node, char *op);
t_optype	set_op_type(char *op);
int			ast_cmdnode(t_ast **node, t_token *start, t_token *end);
int			create_cmd_node(t_ast **node);
int			handle_indir(t_ast **node, t_token *start, t_token *end);
int			is_indir_error(t_token *iterator, t_token *end);
int			set_indir(t_ast **node, t_token *iterator);
int   	 	set_node_infile(t_ast **node, t_token *iterator);
int  		set_node_heredoc(t_ast **node, t_token *iterator);
char		*set_heredoc_name();
int			create_heredoc(char *heredoc, char *heredoc_end);
int  	 	set_node_outfile(t_ast **node, t_token *iterator, t_outstyle style);
int			set_cmdnode_args(t_ast **node, t_token *start, t_token *end);
int     	cmd_args_count(t_token *start, t_token *end);

// exec
int		execute_ast(t_ast **node, t_mnsh *mnsh);
int		set_mnsh_last_arg(t_ast **node, t_mnsh *mnsh);
int		exec_ast_cmd(t_ast **node, t_mnsh *mnsh);
int		exec_ast_cmd_external(char **args, t_mnsh *mnsh);
int		check_and_execute_cmd(char **args, t_mnsh *mnsh);
int		set_execfile(char **execfile, char **args, t_mnsh *mnsh);
int		check_execfile(char *execfile, char **args);
int		get_cmd_path(char **execfile, char *cmd, char **paths);
int		ft_execve(char **execfile, char **args, t_mnsh *mnsh);

int		exec_ast_op(t_ast **node, t_optype op, t_mnsh *mnsh);
int		exec_ast_op_and(t_ast **node, t_mnsh *mnsh);
int		exec_ast_op_or(t_ast **node, t_mnsh *mnsh);
int		exec_ast_op_pipe(t_ast **node, t_mnsh *mnsh);
int		left_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh);
int		right_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh);

int		set_exec_indir(t_ast **node, int *fd_in, int *fd_out);
int		exec_ast_cmd_in(t_ast **node, int *fd);
int		set_last_infile(t_list *infiles, t_list **last_infile);
int		exec_ast_cmd_infile(char *last_infile, int *fd);
int		exec_ast_cmd_out(t_ast **node, int *fd);
int		set_last_outfile(t_list *outfiles, t_list **last_outfile);
int		create_outfile(t_outfile *outfile);
int		exec_ast_cmd_outfile(t_outfile *outfile, int *fd);


// builtins
bool	is_builtin(char *s);
int		exec_ast_cmd_builtin(char **args, t_mnsh *mnsh);

int		mnsh_echo(char **args);
bool	is_echo_option_valid(char *arg);
int		mnsh_env(t_mnsh *mnsh);
int		mnsh_pwd(void);

int		mnsh_cd(char **args, t_mnsh *mnsh);
int		set_target(char **args, char **target, t_mnsh *mnsh);
int		set_cwd(char **cwd);
int		update_pwd_oldpwd(char *cwd, t_mnsh *mnsh);

int		mnsh_export(char **args, t_mnsh *mnsh);
int		print_export_var(t_list *env);
int		handle_export_var(char *arg, t_mnsh *mnsh);
int		set_export_key_value(char *arg, int i, char **key, char **value);
int		update_export_var(char *key, char *value, t_mnsh *mnsh);

int		mnsh_unset(char **args, t_mnsh *mnsh);
void	del_node(t_list **list, char *key);

int		mnsh_exit(char **args, t_mnsh *mnsh);
bool	strtoll_isnum(char *str, long long *n);

// Utils
bool	ft_isspace(char c);
bool	isquote(char c);
bool	ft_isspecial(char c, const char *list_specials);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strarrlen(char **arr);
char	*ft_strappend_mnsh(char *s1, char *s2);
int		load_message(int size, char *msg, int speed);
int		errno_to_exit(int errnum);
int		perror_mnsh(int errnum, int count, ...);
int		perror2_mnsh(int errnum, int count, ...);
char    **env_lst_to_arr(t_list *env_list);
char	*env_var_to_char(t_var *var);

// helpers
void	print_node(t_ast *node);
void	print_ast(t_ast *node, int depth);
void	print_strarray(char *name, char **arr);
void	print_strarray_raw(char **arr, char sep);
void	print_strarray_endl(char *name, char **arr);
void	print_env(char **env);
void	print_infiles(t_list *infiles);
void	print_outfiles(t_list *outfiles);
void	print_tokis(t_token	*tokis);

// Free management
void	ft_free_str(char **ptr);
void	safe_free_str(void *ptr);
void	ft_free_strarray(char ***arr);
void	ft_free_all_tok(t_token **tok);
void	free_outfile(void *ptr);
void	ft_free_ast(t_ast **root_node);
int		free_ast_ret(t_ast **root_node, int errnum);
void	ft_free_reset_mnsh(t_mnsh *mnsh);
void	ft_free_all_mnsh(t_mnsh *mnsh);

#endif
