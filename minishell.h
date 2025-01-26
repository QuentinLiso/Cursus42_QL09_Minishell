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

typedef struct s_exec_loop
{
	int		tmpin;
	int		tmpout;
	int		fdin;
	int		fdout;
	int		fdpipe[2];
	char	*redirect_file_path;
	char	*right_path;
}	t_exec_loop;

typedef struct s_mem
{
	char		**my_env;
	char		**path_tab;
	char		**exec_path;
	char		*path_env;
	int			exit_statue;
	int			last_cmd_exit_statue;
	int			fd_heredocs[2];
	char		*tmpfile;
	t_exec_loop	*exec_loop;
}	t_mem;

typedef struct s_outlst	t_outlst;
struct s_outlst
{
	char		*filename;
	int			action;
	t_outlst	*next;
};

typedef struct s_cmdlst	t_cmdlst;
struct s_cmdlst
{
	char		*command;
	char		**args;
	char		**infiles;
	t_outlst	*outfiles;
	char 		**heredocs;
	int			todo_next;
	t_cmdlst	*next;
};

// 1.c
t_mem	*init_mem(void);
char	**copy_env(char **env);
char	*take_input(t_mem *mem);
char	*get_prompt(t_mem *mem);
char	*my_getenv(char **env, char *elem);
int		strcmp_len(char *s1, char *s2);

// A classer	
t_cmdlst	*parsing(char *command);
int			split_args(t_list **args, char *cmd);
bool		is_special(char c);

// 2.c
int			check_quotes(char	*command);
int			set_in_quotes(char c, int *in_quotes);
int			print_error(int error_code, char *content);

#endif
