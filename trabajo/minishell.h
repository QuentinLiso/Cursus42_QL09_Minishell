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
# include <sys/ioctl.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "libft/includes/libft.h"

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

typedef enum	e_redirstyle
{
	REDIR_NULL,
	REDIR_IN,
	REDIR_HEREDOC,
	REDIR_OUT,
	REDIR_APPEND,
}	t_redirstyle;

typedef struct s_redir
{
	t_redirstyle	style;
	char			*file;
	int				fd_heredoc_read;
}	t_redir;


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

typedef struct s_ast
{
	t_nodetype		node_type;
	t_optype		op_type;
	char			**args;
	t_list			*redir;
	struct s_ast	*left_node;
	struct s_ast	*right_node;
}	t_ast;

typedef struct s_minishell
{
	t_list	*env_mnsh_lst;
	t_sa	sa;
	char	*prompt;
	t_token	*tokis;
	t_token	*last_tokis;
	t_ast	*node;
	int		last_exit_status;
	char	*last_cmd_arg;
	int		line_count;
}	t_mnsh;


// init
int		mnsh_initialization(t_mnsh *mnsh, int ac, char **env);
void	init_mnsh_struct(t_mnsh *mnsh);
int		set_mnsh_env(t_list **env_lst, char **env);
int		set_mnsh_empty_env(t_list **env_lst);
int		set_mnsh_env_shlvl(t_list **env_lst);

// env
t_var	*alloc_env_var(char *key_eq_val);
char	*get_env_var(t_list *env_lst, char *key);
t_list	*get_env_var_prev(t_list *env_lst, char *key);
int		add_env_var(t_list **env_lst, char *key, char *value);
int		edit_env_var(t_list **env_lst, char *key, char *value);
void	display_env_var(void *ptr);

// signals
void	init_sigaction(t_sa *sa, void (*action)(int, siginfo_t *, void *));
void	handle_signal(int signum, siginfo_t *info, void *other);

// loop
int		loop_mnsh(t_mnsh *mnsh);
void	mnsh_prompt(char **prompt);

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
int			create_ast(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh);
t_token		*set_split_token(t_token *start, t_token *end);
int			get_operator_precedence(char *op);
bool		is_syntax_error(t_token *start, t_token *end, t_token *split_token);
int			ast_opnode(t_ast **node, t_token *start, t_token *split_tok, t_token *end, t_mnsh *mnsh);
int 		create_ast_opnode(t_ast **node, char *op);
t_optype	set_op_type(char *op);
int			ast_cmdnode(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh);
int			create_cmd_node(t_ast **node);
int			handle_indir(t_ast **node, t_token *start, t_token *end, t_mnsh *mnsh);
int			is_indir_error(t_token *iterator, t_token *end);
int			set_indir(t_ast **node, t_token *iterator, t_mnsh *mnsh);
int			set_node_redir(t_ast **node, t_token *iterator, t_redirstyle style);
int  		set_node_heredoc(t_ast **node, t_token *iterator, t_mnsh *mnsh);
char		*set_heredoc_name();
int			create_heredoc(t_redir *redir_file, char *heredoc_end, t_mnsh *mnsh);
void	fill_heredoc(int fd, char *heredoc_end, t_mnsh *mnsh);
void	warn_heredoc(int line, char *heredoc_end);
int			set_cmdnode_args(t_ast **node, t_token *start, t_token *end);
int     	cmd_args_count(t_token *start, t_token *end);

// exec
int		execute_ast(t_ast **node, t_mnsh *mnsh);
int		set_mnsh_last_arg(t_ast **node, t_mnsh *mnsh);
int		exec_ast_cmd(t_ast **node, t_mnsh *mnsh);
int		exec_ast_cmd_external(char **args, t_mnsh *mnsh);
int		check_and_execute_cmd(char **args, t_mnsh *mnsh);
int		set_execfile(char **execfile, char **args, t_mnsh *mnsh);
int		set_execfile_paths(t_list *env_mnsh_list, char ***paths);
int		check_execfile(char *execfile, char **args);
int		get_cmd_path(char **execfile, char *cmd, char **paths);
int		ft_execve(char **execfile, char **args, t_mnsh *mnsh);

int		exec_ast_op(t_ast **node, t_optype op, t_mnsh *mnsh);
int		exec_ast_op_and(t_ast **node, t_mnsh *mnsh);
int		exec_ast_op_or(t_ast **node, t_mnsh *mnsh);
int		exec_ast_op_pipe(t_ast **node, t_mnsh *mnsh);
int		left_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh);
int		right_pipe(t_ast **node, int (*fd)[2], int *pid, t_mnsh *mnsh);

int		init_default_fd(int (*default_inout)[2]);
int		clean_default_fd(int (*default_inout)[2]);

int		exec_ast_cmd_indir(t_list *redir);
int		check_access_indir_lst(t_list *redir);
int		check_access_indir_elem(t_redir *elem);
int		create_outfile(t_redir *redir_file);
int		dup_indir_lst(t_list *redir);
int		dup_indir_elem(t_redir *elem, int *fd_in, int *fd_out);
int		dup_indir_elem_in(t_redir *elem, int *fd_in);
int		dup_indir_elem_out(char *file, int *fd_out, int flag);

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

// Free management
void	free_str(char **ptr);
void	free_str_lst(void *ptr);
void	free_strarray(char ***arr);
int		free_env_var_ret(t_var *var, int ret);
void	free_env_var(void *ptr);
void	free_tokis(t_token **tok);
void	free_redir(void *ptr);
void	free_ast_node(t_ast **node);
void	free_reset_mnsh(t_mnsh *mnsh);
void	free_all_mnsh(t_mnsh *mnsh);

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
void	print_redir(t_list *redir_files);
void	print_tokis(t_token	*tokis);
void	check_open_fds(void);

#endif
