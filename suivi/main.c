#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_mem		*mem;
	t_cmdlst	*command_list;
	char		*str;

	(void)ac, (void)av;
	g_pid = 0;
	mem = initialize_mem();				// Minishell Function
	signal(SIGINT, handle_signals);		// Minishell Function
	signal(SIGQUIT, handle_signals);
	mem->path_tab = ft_split(getenv("PATH"), ':');	// Libft function + C Function
	mem->my_env = copy_env(env);		// Minishell Function
	ft_printf("\033[1;1H\033[2J");		// Libft Function
	while (42)
	{
		str = take_input(mem);			// Minishell Function
		if (!str)
			continue ;
		command_list = parsing(str);	// Minishell Function
		free(str);
		if (!command_list)
			continue ;
		execute(command_list, mem);		// Minishell Function
		cmdlst_clear(&command_list);	// Minishell Function
	}
	return (free_mem(mem));				// Minishell Function
}


// ====================================================================== Init + Signals

t_mem	*initialize_mem(void)
{
	t_mem	*mem;

	mem = (t_mem *)malloc(sizeof(t_mem));
	mem->exec_loop = (t_exec_loop *)malloc(sizeof(t_exec_loop));
	init_exec_loop(mem->exec_loop);		// Minishell Function
	mem->my_env = NULL;
	mem->path_tab = NULL;
	mem->tmpfile = "buzz";
	mem->exit_statue = 0;
	mem->last_cmd_exit_statue = 0;
	return (mem);
}

void	init_exec_loop(t_exec_loop *exec_loop)
{
	exec_loop->redirect_file_path = NULL;
	exec_loop->right_path = NULL;
}

/*
	On SIGINT => new prompt
	On SIGQUIT => nothing
*/
void	handle_signals(int signo)
{
	if (g_pid)
		return ;
	if (signo == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_printf("  \b\b");
	}
}

/////////====================================================================== Copy Env

/*
	Return the a copy of env
	Return NULL on malloc error
*/
char	**copy_env(char **env)
{
	char	**my_env;
	int		i;
	int		len;

	len = 0;
	i = 0;
	while (env[i++])
		len++;
	my_env = (char **)malloc(sizeof(char *) * (len + 2));
	if (!my_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		my_env[i] = ft_strdup(env[i]);
		i++;
	}
	my_env[i] = NULL;
	return (my_env);
}


///// ======================================================================= Display input line

char	*take_input(t_mem *mem)
{
	char	*buf;
	char	*prompt;

	prompt = get_prompt(mem);		// Minishell Function
	buf = readline(prompt);
	free(prompt);
	if (!buf)
	{
		ft_printf("\n");
		exit(free_mem(mem));		// Minishell Function + C Function
	}
	if (ft_strlen(buf) == 0)
	{
		free(buf);
		return (NULL);
	}
	add_history(buf);
	return (buf);
}

/*
	Return a string containing the prompt
	That string is malloced, it need to be free
	Return NULL on malloc error
	If PWD exist, return :
		/home/cestoliv/Documents/DOCS/IT/Dev/42cursus/Minishell-42
		>
	If PWD doesn't exist, return :
		minishell >
	The arrow color is red if the previous command failed
*/
char	*get_prompt(t_mem *mem)
{
	char	*prompt;
	char	*pwd;
	char	*temp;
	char	*start;

	pwd = my_getenv(mem->my_env, "PWD");	// Minishell Function
	if (pwd)
	{
		temp = ft_strjoin("\n\001\033[36m\002", pwd);
		free(pwd);
		start = ft_strjoin(temp, "\001\033[0m\002\n");
		free(temp);
	}
	else
		start = ft_strdup("\n\001\033[36m\002minishell\001\033[0m\002 ");
	if (mem->last_cmd_exit_statue == 0)
		prompt = ft_strjoin(start, "\001\033[32m\002>\001\033[0m\002 ");
	else
		prompt = ft_strjoin(start, "\001\033[31m\002>\001\033[0m\002 ");
	free(start);
	return (prompt);
}

int	free_mem(t_mem *mem)
{
	int	exit_status;

	exit_status = mem->last_cmd_exit_statue;
	if (mem->my_env)
		free_tab_2d(mem->my_env);
	if (mem->path_tab)
		free_tab_2d(mem->path_tab);
	if (mem->exec_loop)
		free_exec_loop(mem->exec_loop);
	if (mem)
		free(mem);
	return (exit_status);
}

void	free_tab_2d(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		if (tab[i])
			free(tab[i]);
		i++;
	}
	if (tab)
		free(tab);
}

void	free_exec_loop(t_exec_loop *exec_loop)
{
	if (exec_loop->redirect_file_path)
		free(exec_loop->redirect_file_path);
	if (exec_loop->right_path)
		free(exec_loop->right_path);
	free(exec_loop);
}

/*
	Return the value of the given env name
	Return NULL if the value doesn't exist
	Return NULL on malloc error
*/
char	*my_getenv(char **env, char *elem)
{
	char	*str;
	int		index;
	int		elem_len;

	elem_len = ft_strlen(elem);
	index = 0;
	while (env[index])
	{
		if (!strcmp_len(elem, env[index]))		// Minishell Function
			break ;
		index++;
	}
	if (!env[index])
		return (NULL);
	str = ft_strdup(env[index] + elem_len + 1);
	if (!str)
		return (NULL);
	return (str);
}

/*
	Return 0 when the whole s1 is the same as the start of s2
*/
int	strcmp_len(char *s1, char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s1)));
}


// ================================================ Parsing

/*
	Main parsing function, return a list of commands
	Start by splitting the command in a list of args with split_args(),
	then parse that string list into a list of commands with create_command_lst()
	Return NULL on malloc error
	Return NULL on syntax error
*/
t_cmdlst	*parsing(char *command)
{
	t_cmdlst	*command_list;
	t_list		*args;

	args = NULL;
	if (check_quotes(command))				// Minishell Function
		return (NULL);
	if (!split_args(&args, command))		// Minishell Function
		return (NULL);
	if (check_specials(args))				// Minishell Function
	{
		lst_clear(&args);					// Minishell Function
		return (NULL);
	}
	if (!create_command_lst(&command_list, args))	// Minishell Function
	{
		lst_clear(&args);					// Minishell Function
		return (NULL);
	}
	lst_clear(&args);						// Minishell Function
	return (command_list);
}

// ================================================ Parsing : checking quotes

/*
	Return 1 on syntax error, 0 if there is no error
	Error : An openned quote in not closed
*/
int	check_quotes(char *command)
{
	int	cur;
	int	in_quotes;

	cur = 0;
	in_quotes = 0;
	while (command[cur])
	{
		set_in_quotes(command[cur], &in_quotes);	// Minishell Function
		cur++;
	}
	if (in_quotes != 0)
		print_error(1, NULL);						// Minishell Function
	return (in_quotes != 0);
}

/*
	in_quotes is a var that is equal to 1 when a single quote is openned,
	2 if a double quote is openned or 0 when quotes are closed.
	The function set given in_quotes to 0, 1 or 2 depending on the
	value of given char and the current in_quotes value.
	Return the previous value of in_quotes
*/
int	set_in_quotes(char c, int *in_quotes)
{
	int	cquotes;

	cquotes = *in_quotes;
	if (c == '\'')
	{
		if (cquotes == 0)
			*in_quotes = 1;
		else if (cquotes == 1)
			*in_quotes = 0;
	}
	if (c == '"')
	{
		if (cquotes == 0)
			*in_quotes = 2;
		else if (cquotes == 2)
			*in_quotes = 0;
	}
	return (cquotes);
}

/*
	Print the error corresponding to the given error code
*/
static int	print_error(int error_code, char *content)
{
	if (error_code == 1)
	{
		ft_printf("\033[91m%s\033[0m\n",
			"minishell: parse error, quotes are never closed");
	}
	else if (error_code == 2)
	{
		ft_printf("\033[91m%s '%s'\033[0m\n",
			"minishell: parse error, near", content);
	}
	else if (error_code == 3)
	{
		ft_printf("\033[91m%s '%s'\033[0m\n",
			"minishell: the command cannot end with",
			content);
	}
	return (1);
}


// ================================================  Parsing : split args

/*
	Split the given command in the given args list
	Works like a split on spaces, but it also split on <, <<, >, >>, &&, |, ||, ;
	Of course, it doesn't split on these characters if they are surrounded
	by simple or double quotes

	E.g.: `echo 'Hello World' && cat -n 5 Makefile`
			return : ["echo", "'Hello World'", "&&", "cat", "-n", "5", "Makefile"]
*/
int	split_args(t_list **args, char *cmd)
{
	int		cur;
	char	*arg;
	int		end_index;

	cur = -1;
	while (cmd[++cur])
	{
		if (cmd[cur] == ' ' || cmd[cur] == '\t')
			continue ;
		if (cmd[cur] == '\'' || cmd[cur] == '"' || cmd[cur] == '<'
			|| cmd[cur] == '&' || cmd[cur] == '|' || cmd[cur] == '>'
			|| cmd[cur] == ';')
			end_index = get_end_index(cmd + cur, -1);				// Minishell function
		else
			end_index = get_end_index(cmd + cur, get_arg_end(cmd + cur, 0));	// Minishell function + Minishell function
		if (!end_index)
			return (lst_clear(args));				// Minishell function
		arg = ft_strldup(cmd + cur, end_index);		// Minishell function
		if (!arg)
			return (lst_clear(args));				// Minishell function
		if (!lst_append(args, arg))					// Minishell function
			return (lst_clear(args));				// Minishell function
		cur += ft_strlen(arg) - 1;
	}
	return (1);
}

/*
	Return the end of the first arg of given str
	e_end is equal to the return of get_arg_end
		on the current string (with a quote_index = 0)
	A e_end = to -1 means that str start with a special char
		like quotes, &, |, <, > or ;
	It can find if the arg is actually longer than the e_end
		because of some quotes (with get_quotes_end)
	or if it's lower because of a &&, ||, |, <<, <, >>, > or ; before
*/
int	get_end_index(char *str, int e_end)
{
	int		end_index;
	char	**splitchar;
	int		cur;

	if (e_end == 0)
		return (0);
	splitchar = ft_split("&& || | << < >> > & ;", ' ');
	if (!splitchar)
		return (0);
	end_index = get_quotes_end(str, e_end);				// Minishell Function
	if (end_index == 0)
		return (strarr_free(splitchar));				// Minishell Function
	if (end_index != -2)
		return (end_index + strarr_free(splitchar));	// Minishell Function
	cur = -1;
	while (splitchar[++cur])
	{
		if ((e_end > -1 && index_of(str, splitchar[cur], 1) < e_end))			// Minishell Function
			return (index_of(str, splitchar[cur], 1) + strarr_free(splitchar));	// Minishell Function + Minishell Function
		if (index_of(str, splitchar[cur], 1) == 0)								// Minishell Function
			return (ft_strlen(splitchar[cur]) + strarr_free(splitchar));		// Minishell Function + Minishell Function
	}
	return (e_end + strarr_free(splitchar));									// Minishell Function
}

/*
	Return the end of the first arg of given str
	It's like a wrapper for get_arg_end() that jump directly to the closing quotes
	It return get_arg_end() with the index of the closing quote
		(if the arg starts with quotes, index_of need to find the first quote,
		if it doesn't start with quotes but contains quotes,
			index_of need to find the second quote, because the first is the opening quote
	Return -2 if there is not any quotes
*/
int	get_quotes_end(char *str, int e_end)
{
	int	end_index;

	end_index = -2;
	if (str[0] == '\'')
		end_index = get_arg_end(str, index_of(str + 1, "'", 1) + 2);			// Minishell Function
	else if (e_end > -1 && index_of(str, "'", 1) < e_end)						// Minishell Function
		end_index = get_arg_end(str, index_of(str + 1, "'", 2) + 2);			// Minishell Function
	else if (str[0] == '"')	
		end_index = get_arg_end(str, index_of(str + 1, "\"", 1) + 2);			// Minishell Function
	else if (e_end > -1 && index_of(str, "\"", 1) < e_end)						// Minishell Function
		end_index = get_arg_end(str, index_of(str + 1, "\"", 2) + 2);			// Minishell Function
	return (end_index);
}

/*
	Return the index of the n-th occurence of the
	given search in the given str
*/
int	index_of(char *str, char *search, int n)
{
	int	cur;

	cur = 0;
	while (str[cur])
	{
		if (ft_strncmp(str + cur, search, ft_strlen(search)) == 0)
			n--;
		if (n <= 0)
			return (cur);
		cur++;
	}
	return (cur);
}

/*
	Return the end of the first arg of given str
	It's gonna find the end index of the arg after the quotes
		(it takes the closing quote index as parameter)
	It return the index of the first space (or tab) except if that space
	is after a &&, ||, |, <<, <, >>, > or ; it's gonna return the
	index just before that characters
*/
int	get_arg_end(char *str, int quote_index)
{
	char	**splitchar;
	int		end_index;
	int		test_index;
	int		cur;

	splitchar = ft_split("&& || | << < >> > & ;", ' ');
	if (!splitchar)
		return (0);
	end_index = index_of_newline(str + quote_index);							// Minishell Function
	cur = -1;
	while (splitchar[++cur])
	{
		test_index = index_of(str + quote_index, splitchar[cur], 1);			// Minishell Function
		if (test_index < end_index)
		{
			strarr_free(splitchar);												// Minishell Function
			return (test_index + quote_index);
		}
	}
	strarr_free(splitchar);
	return (end_index + quote_index);
}

/*
	Return the index of the first space or tab of given str
*/
int	index_of_newline(char *str)
{
	int	index;
	int	temp;

	index = index_of(str, " ", 1);
	temp = index_of(str, "\t", 1);
	if (temp || str[0] == '\t')
		if (temp < index)
			index = temp;
	return (index);
}

/*
	Free an str array
*/
int	strarr_free(char **array)
{
	int	cur;

	cur = -1;
	if (array)
	{
		while (array[++cur])
			free(array[cur]);
		free(array);
	}
	return (0);
}

/*
	Free a t_list
*/
int	lst_clear(t_list **lst)
{
	t_list	*next;

	while (*lst)
	{
		next = (*lst)->next;
		free((*lst)->content);
		free(*lst);
		*lst = next;
	}
	*lst = NULL;
	return (0);
}

/*
	Same as ft_strdup, but with a size params to limit
	the length of the future string.
*/
char	*ft_strldup(const char *s1, int size)
{
	int		s1_len;
	char	*dup;

	s1_len = ft_strlen(s1);
	if (s1_len > size)
		s1_len = size;
	dup = (char *)malloc((s1_len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	dup[s1_len] = 0;
	while (--s1_len >= 0)
		dup[s1_len] = s1[s1_len];
	return (dup);
}

/*
	Create a new lst element with the given str
	and append it at the end of the given list
	Return 0 on malloc error
*/
int	lst_append(t_list **lst, char *str)
{
	t_list	*new;

	new = ft_lstnew(str);
	if (!new)
		return (lst_clear(&new));
	ft_lstadd_back(lst, new);
	return (1);
}


// ================================================ Parsing : Check specials

/*
	Return 1 on syntax error, 0 if there is no error
	Error :	A special is followed by a special
			The command ends with a special

	E.g.:	echo 42 > && cat Makefile
			echo 42 &&
*/
int	check_specials(t_list *args)
{
	t_list	*it_args;
	int		is_special;

	it_args = args;
	is_special = 0;
	while (it_args)
	{
		if (is_sep(it_args->content))
		{
			if (is_special)
				return (print_error(2, it_args->content));
			is_special = 1;
		}
		else
			is_special = 0;
		if (!it_args->next)
		{
			if (is_sep(it_args->content) && strcmp_len(it_args->content, ";"))
				return (print_error(3, it_args->content));
		}
		it_args = it_args->next;
	}
	return (0);
}

/*
	Return 1 when str is a separator
	A separator can be : |, &, ||, &&, >, >>, <, <<, ;
*/
int	is_sep(char *str)
{
	if (get_arg_type(str) != 0)
		return (1);
	if (!strcmp_len(str, ">"))
		return (1);
	if (!strcmp_len(str, "<"))
		return (1);
	if (!strcmp_len(str, "<<"))
		return (1);
	if (!strcmp_len(str, ">>"))
		return (1);
	if (!strcmp_len(str, "&"))
		return (1);
	if (!strcmp_len(str, ";"))
		return (1);
	return (0);
}

// ================================================ Parsing : Create command list

/*
	Create a command list from an args list
	Return 0 on malloc error
*/
int	create_command_lst(t_cmdlst **command_list, t_list *args)
{
	t_cmdlst	*new;

	*command_list = NULL;
	while (args && !get_arg_type(args->content))
	{
		new = cmdlst_new();
		if (!new)
			return (cmdlst_clear(command_list));
		if (*command_list)
			cmdlst_last(*command_list)->next = new;
		else
			*command_list = new;
		new->command = ft_strdup(args->content);
		if (!new->command)
			return (cmdlst_clear(command_list));
		if (!cmdlist_append_args(&args, new))
			return (cmdlst_clear(command_list));
		if (args)
			new->todo_next = get_arg_type(args->content);
		if (args)
			args = args->next;
	}
	return (1);
}

/*
	Return NEXT_PIPE, NEXT_AND, NEXT_OR, NEXT_END or NEXT_CONTINUE
	depending of the value of str (|, &&, ||, ; or someting else)
*/
int	get_arg_type(char *str)
{
	if (!strcmp_len(str, "|"))
		return (NEXT_PIPE);
	if (!strcmp_len(str, "&&"))
		return (NEXT_AND);
	if (!strcmp_len(str, "||"))
		return (NEXT_OR);
	if (!strcmp_len(str, ";"))
		return (NEXT_CONTINUE);
	return (NEXT_END);
}

/*
	Create a new empty cmdlst element
	Return 0 on malloc error
*/
t_cmdlst	*cmdlst_new(void)
{
	t_cmdlst	*new;

	new = malloc(sizeof(t_cmdlst));
	if (!new)
		return (0);
	new->command = NULL;
	new->next = NULL;
	new->todo_next = NEXT_END;
	new->args = NULL;
	new->infiles = NULL;
	new->outfiles = NULL;
	new->heredocs = NULL;
	return (new);
}

/*
	Free a command list
*/
int	cmdlst_clear(t_cmdlst **lst)
{
	t_cmdlst	*next;

	while (*lst)
	{
		next = (*lst)->next;
		free((*lst)->command);
		strarr_free((*lst)->args);
		outlst_clear(&(*lst)->outfiles);
		strarr_free((*lst)->infiles);
		strarr_free((*lst)->heredocs);
		free(*lst);
		*lst = next;
	}
	*lst = NULL;
	return (0);
}

/*
	Free an outlist (mostly used by cmdlst_clear)
*/
int	outlst_clear(t_outlst **lst)
{
	t_outlst	*next;

	while (*lst)
	{
		next = (*lst)->next;
		free((*lst)->filename);
		free(*lst);
		*lst = next;
	}
	*lst = NULL;
	return (0);
}

/*
	Return a pointer to the last element of the given
	command list
*/
t_cmdlst	*cmdlst_last(t_cmdlst *lst)
{
	if (lst)
		while (lst->next)
			lst = lst->next;
	return (lst);
}

/*
	Append args->content to the right array / list
	Return 0 on malloc error
	If args->content is equal to > or >>
		It append args->next->content to the outfiles list
		-> using append_out_args
	If args->content is equal to <
		It append args->next->content to the infiles array
		-> using append_in_args
	If args->content is equal to <<
		It append args->next->content to the heredocs array
		-> using append_in_args
	Else
		It append args->content to the args array
		-> using strarr_append
*/
int	cmdlist_append_args(t_list **args, t_cmdlst *new)
{
	while (*args && !get_arg_type((*args)->content))
	{
		if (!append_out_args(args, ">", &(new->outfiles)))
			return (0);
		else if (!append_in_args(args, "<", &(new->infiles)))
			return (0);
		else if (!append_out_args(args, ">>", &(new->outfiles)))
			return (0);
		else if (!append_in_args(args, "<<", &(new->heredocs)))
			return (0);
		if ((*args) && !is_sep((*args)->content))
		{
			if (!strarr_append(&(new->args), (*args)->content))
				return (0);
			*args = (*args)->next;
		}
	}
	return (1);
}

/*
	Append the next arg of given args list to the given array.
	Return 0 on malloc error
	array can be infiles or heredoc.
	If the args->content is < or <<, it append args->next->content
	to the given array using strarr_append
*/
int	append_in_args(t_list **args, char *op, char ***array)
{
	if (!(*args))
		return (1);
	if (ft_strlen(op) == ft_strlen((*args)->content)
		&& !strcmp_len((*args)->content, op))
	{
		*args = (*args)->next;
		if (*args && !strarr_append(array, (*args)->content))
			return (0);
		*args = (*args)->next;
	}
	return (1);
}

/*
	Append the next arg of given args list to the given outfiles list.
	Return 0 on malloc error
	If the args->content is > or >>, it append args->next->content
	to the given list using outlst_append
*/
int	append_out_args(t_list **args, char *op, t_outlst **out)
{
	if (!(*args))
		return (1);
	if (ft_strlen(op) == ft_strlen((*args)->content)
		&& !strcmp_len((*args)->content, op))
	{
		*args = (*args)->next;
		if (*args && !outlst_append(out, (*args)->content, op))
			return (0);
		*args = (*args)->next;
	}
	return (1);
}

/*
	Create a new outlist element (using given filename and spe) using
	outlst_new.
	It convert the given spe to an int (> = OUT_WRITE, >> = OUT_APPEND).
	Then append it at the end of the given lst.
	Return 0 on malloc error
*/
int	outlst_append(t_outlst **lst, char *filename, char *spe)
{
	t_outlst	*new;
	int			action;

	action = OUT_WRITE;
	if (ft_strlen(spe) == 2)
		action = OUT_APPEND;
	new = outlst_new(filename, action);
	if (!new)
		return (0);
	if (*lst)
		outlst_last(*lst)->next = new;
	else
		(*lst) = new;
	return (1);
}

/*
	Create a new outlst element with the given filename and action.
	It dulpicate the filename with ft_strdup
	Return 0 on malloc error
*/
t_outlst	*outlst_new(char *filename, int action)
{
	t_outlst	*new;

	new = malloc(sizeof(t_outlst));
	if (!new)
		return (0);
	new->filename = ft_strdup(filename);
	if (!new->filename)
	{
		free(new);
		return (0);
	}
	new->action = action;
	new->next = NULL;
	return (new);
}

/*
	Return a pointer to the last element of the given
	outfiles list
*/
t_outlst	*outlst_last(t_outlst *lst)
{
	if (lst)
		while (lst->next)
			lst = lst->next;
	return (lst);
}

/*
	Append the given str to the end of the given array
	It malloc an entire new array, duplicates the existing elements
	and duplicate the given string. Then it free the original
	array and replace it's pointer to the new array's one
	Return 0 on malloc error
*/
int	strarr_append(char ***array, char *str)
{
	char	**new;
	int		new_len;
	int		cur;

	new_len = strarr_len(*array) + 1;
	new = malloc(sizeof(char *) * (new_len + 1));
	if (!new)
		return (0);
	cur = 0;
	while ((*array) && (*array)[cur])
	{
		new[cur] = ft_strdup((*array)[cur]);
		if (!new[cur])
			return (free_array_n(new, cur));
		cur++;
	}
	new[cur] = ft_strdup(str);
	if (!new[cur])
		return (free_array_n(new, cur));
	cur++;
	new[cur] = 0;
	strarr_free(*array);
	*array = new;
	return (1);
}

/*
	Return the len of an str array (that is 0-terminated)
*/
int	strarr_len(char **array)
{
	int	cur;

	cur = 0;
	if (array)
		while (array[cur])
			cur++;
	return (cur);
}

/*
	Free only the n-first elements of an str array
*/
int	free_array_n(char **array, int n)
{
	while (n--)
		free(array[n]);
	free(array);
	return (0);
}

// ================================================ Execute 

/*
	loop to execute one or multiple command in lst
*/
void	execute(t_cmdlst *lst, t_mem *mem)
{
	int	i;

	mem->exec_loop->redirect_file_path = NULL;
	init_exec(mem, lst);
	while (lst)
	{
		setup_infiles(lst, mem);
		if (is_exiting(lst))
			ft_exit(mem, lst);
		replace_quotes(&(lst->args), mem->my_env, mem->last_cmd_exit_statue);
		if (outlst_len(lst->outfiles) > 0)
			i = 0;
		else
			i = -1;
		while (i < outlst_len(lst->outfiles))
		{
			exec_next(lst, mem, i);
			i++;
		}
		if (save_last_exit_statue(mem, lst))
			lst = lst->next;
		lst = lst->next;
	}
	reset_exec(mem);
}

// ================================================ Execute : Initialization

/*
	init var for exec loop
	call before exec loop or at the end of loop when next_to_do is && or ||
*/
void	init_exec(t_mem *mem, t_cmdlst *lst)
{
	mem->exec_loop->tmpin = dup(0);
	mem->exec_loop->tmpout = dup(1);
	if (lst->infiles)
		mem->exec_loop->fdin = open(lst->infiles[0], O_RDONLY);
	else
		mem->exec_loop->fdin = dup(mem->exec_loop->tmpin);
	mem->exit_statue = 0;
}

// ================================================ Execute : Infiles

/*
	set up infiles (< et <<)
*/
void	setup_infiles(t_cmdlst *lst, t_mem *mem)
{
	if (lst->infiles)
		mem->exec_loop->fdin = open(lst->infiles[0], O_RDONLY);
	if (lst->heredocs)
	{
		reset_exec(mem);
		init_exec(mem, lst);
		handle_heredocs(mem, lst);
	}
}

/*
	reset var for exec loop
	call after exec loop or at the end of loop when next_to_do is && or ||
*/
void	reset_exec(t_mem *mem)
{
	dup2(mem->exec_loop->tmpin, 0);
	dup2(mem->exec_loop->tmpout, 1);
	close(mem->exec_loop->tmpin);
	close(mem->exec_loop->tmpout);
	if (mem->exec_loop->redirect_file_path)
		free(mem->exec_loop->redirect_file_path);
	mem->exec_loop->redirect_file_path = NULL;
	mem->exec_loop->right_path = NULL;
	mem->exit_statue = 0;
}

/*
	if there is a << in command
	open a tmpfile
	new prompt, write buf in it
*/
void	handle_heredocs(t_mem *mem, t_cmdlst *lst)
{
	int		fd;
	char	*buf;

	fd = open(mem->tmpfile, O_TRUNC | O_CREAT | O_WRONLY, 0777);
	buf = readline("heredocs>>> ");
	while (1)
	{
		if (ft_strcmp(buf, lst->heredocs[0]) == 0)
			break ;
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		buf = readline("heredocs>>> ");
	}
	free(buf);
	close(fd);
	mem->exec_loop->fdin = open(mem->tmpfile, O_RDONLY, 0777);
	dup2(mem->exec_loop->fdin, STDIN_FILENO);
	close(mem->exec_loop->fdin);
}

// ================================================ Execute : Exit minishell

int	is_exiting(t_cmdlst *lst)
{
	if ((ft_strcmp(lst->command, "exit") == 0) && ((lst->todo_next == 0
				&& cmdlist_len(lst) == 1) || lst->todo_next == 2
			|| lst->todo_next == 3))
		return (1);
	return (0);
}

void	ft_exit(t_mem *mem, t_cmdlst *lst)
{
	cmdlst_clear(&lst);
	exit(free_mem(mem));
}

// ================================================ Execute : Quotes

/*
	Replace the env vars of every args in given array using replace_in_arg.
	It malloc a whole new array and duplicate every args (and replace env vars
	when needed), then it free the old arg array and set the args pointer
	to the new array.
	Return 0 on malloc error
*/
int	replace_quotes(char ***args, char **env, int last_exit)
{
	char	**new_args;
	int		cur;

	new_args = malloc(sizeof(char *) * (strarr_len(*args) + 1));
	if (!new_args)
		return (0);
	cur = 0;
	while ((*args)[cur])
	{
		new_args[cur] = replace_in_arg((*args)[cur], env, last_exit);
		if (!new_args[cur])
			return (free_array_n(new_args, cur));
		cur++;
	}
	new_args[cur] = 0;
	strarr_free(*args);
	*args = new_args;
	return (1);
}

/*
	Return the len of an str array (that is 0-terminated)
*/
int	strarr_len(char **array)
{
	int	cur;

	cur = 0;
	if (array)
		while (array[cur])
			cur++;
	return (cur);
}

/*
	Replace the env vars of the given arg and return the
	modified arg.
	lex stand for last exit
	Return NULL on malloc error
*/
char	*replace_in_arg(char *arg, char **env, int lex)
{
	char	*n_arg;
	int		cur;
	int		in_quotes;
	int		n_cur;
	char	*var_val;

	n_arg = malloc_new_arg(arg, env, lex);
	if (!n_arg)
		return (NULL);
	cur = -1;
	n_cur = 0;
	in_quotes = 0;
	while (arg[++cur])
	{
		if (set_in_quotes(arg[cur], &in_quotes) == in_quotes)
			n_arg[n_cur++] = arg[cur];
		if (in_quotes != 1 && arg[cur] == '$')
		{
			var_val = get_env(arg + cur, env, lex);
			if (!replace_var(var_val, n_arg, &n_cur))
				return (NULL);
			cur += get_envvar_size(arg + cur) - 1;
		}
	}
	return (n_arg);
}

// ================================================ Execute : Execute next

int	outlst_len(t_outlst *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

/*
	hearts of exec that loop over the number of outfiles
	create a child process with fork
	exec cmd
*/
void	exec_next(t_cmdlst *lst, t_mem *mem, int i)
{
	init_outlst_loop(mem, lst, i);
	g_pid = fork();
	if (g_pid == 0)
		exec_cmd(mem, lst);
	waitpid(g_pid, &(mem->exit_statue), 0);
	g_pid = 0;
	mem->exit_statue = WEXITSTATUS(mem->exit_statue);
	if ((ft_strcmp(lst->command, "cd") == 0 && lst->todo_next != 1)
		|| (ft_strcmp(lst->command, "export") == 0 && lst->todo_next != 1)
		|| (ft_strcmp(lst->command, "unset") == 0 && lst->todo_next != 1))
		do_builtin(lst, mem);
	if (lst->heredocs)
		delete_tpmfile(mem);
}

// ================================================ Execute : Execute next : Init outlst loop

/*
   init the loop of outfiles
*/
void	init_outlst_loop(t_mem *mem, t_cmdlst *lst, int i)
{
	dup2(mem->exec_loop->fdin, 0);
	close(mem->exec_loop->fdin);
	set_up_output(mem, lst, i);
	dup2(mem->exec_loop->fdout, 1);
	close(mem->exec_loop->fdout);
}

/*
	if next is not a pipe
		-> if outfiles (> or >>)
		-> else
	else
		-> create a pipe
		it's all about where u need to put the output :)
*/
void	set_up_output(t_mem *mem, t_cmdlst *lst, int i)
{
	if (i > 0)
		lst->outfiles = lst->outfiles->next;
	if (lst->todo_next != 1)
	{
		if (lst->outfiles)
			set_up_outfiles(mem, lst);
		else
			mem->exec_loop->fdout = dup(mem->exec_loop->tmpout);
	}
	else
	{
		pipe(mem->exec_loop->fdpipe);
		mem->exec_loop->fdout = mem->exec_loop->fdpipe[1];
		mem->exec_loop->fdin = mem->exec_loop->fdpipe[0];
	}
}

void	set_up_outfiles(t_mem *mem, t_cmdlst *lst)
{
	if (lst->outfiles->action == 0)
	{
		mem->exec_loop->redirect_file_path
			= find_path_redirect_file(getenv("PWD"),
				lst->outfiles->filename, mem->exec_loop->redirect_file_path);
			mem->exec_loop->fdout = open(mem->exec_loop->redirect_file_path,
				O_CREAT | O_WRONLY | O_TRUNC, S_IRWXO | S_IRWXU | S_IRWXG);
	}
	else if (lst->outfiles->action == 1)
	{
		mem->exec_loop->redirect_file_path
			= find_path_redirect_file(getenv("PWD"),
				lst->outfiles->filename, mem->exec_loop->redirect_file_path);
			mem->exec_loop->fdout = open(mem->exec_loop->redirect_file_path,
				O_APPEND | O_CREAT | O_WRONLY, S_IRWXO | S_IRWXU | S_IRWXG);
	}
}

/*
	return redirect_file_path for outfiles
*/
char	*find_path_redirect_file(char *pwd, char *actuel, char *redirect_path)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	redirect_path = malloc(sizeof(char) * (ft_strlen(pwd)
				+ ft_strlen(actuel) + 3));
	if (!redirect_path)
		return (NULL);
	while (pwd[i])
		redirect_path[j++] = pwd[i++];
	redirect_path[j] = '/';
	j++;
	i = 0;
	while (actuel[i])
		redirect_path[j++] = actuel[i++];
	redirect_path[j] = '\0';
	return (redirect_path);
}

// ================================================ Execute : Execute next : Built in pwd, env, echo + non built in

void	exec_cmd(t_mem *mem, t_cmdlst *lst)
{
	init_exec_cmd(mem, lst);
	if (is_builtin(lst->command))
	{
		if (!exec_builtin(mem, lst))
			exit(0);
	}
	else
	{
		if (execve(mem->exec_path, lst->args, mem->my_env) == -1)
		{
			if (errno == 14 || errno == 13)
				print_error(errno, lst->command);
			else
				ft_printf("%d %s\n", errno, strerror(errno));
			if (errno == 14)
				exit(127);
			free_path(mem);
			exit(1);
		}
	}
	free_path(mem);
	exit(0);
}

/*
   if builtin cmd do things for
   if not builtin call execve
*/
void	init_exec_cmd(t_mem *mem, t_cmdlst *lst)
{
	mem->exec_path = NULL;
	mem->path_env = my_getenv(mem->my_env, "PATH");
	if (mem->path_tab)
		free_tab_2d(mem->path_tab);
	mem->path_tab = ft_split(mem->path_env, ':');
	mem->exec_path = find_exec_path(mem->path_tab, lst->command,
			mem->exec_path);
}

/*
	return a path used as first parameter of execve fct
*/
char	*find_exec_path(char **path_tab, char *cmd, char *right_path)
{
	int	i;

	if (cmd[0] == '.' || cmd[0] == '/')
		return (ft_strdup(cmd));
	if (!path_tab)
		return (NULL);
	i = 0;
	while (path_tab[i])
	{
		right_path = try_path(path_tab, cmd, right_path, i);
		if (right_path)
			return (right_path);
		i++;
	}
	return (NULL);
}

char	*try_path(char **path_tab, char *cmd, char *right_path, int i)
{
	int	j;
	int	x;

	j = 0;
	x = 0;
	right_path = malloc(sizeof(char) * 150);
	while (path_tab[i][j])
		right_path[x++] = path_tab[i][j++];
	right_path[x] = '/';
	x++;
	j = 0;
	while (cmd[j])
		right_path[x++] = cmd[j++];
	right_path[x] = '\0';
	if (access(right_path, F_OK) == 0)
		return (right_path);
	free(right_path);
	return (NULL);
}

/*
	Return 2 if is builtin and change env
	Return 1 if is builtin
	return 0 if not builtin
*/
int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (2);
	if (ft_strcmp(cmd, "exit") == 0)
		return (2);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (2);
	if (ft_strcmp(cmd, "unset") == 0)
		return (2);
	return (0);
}

int	exec_builtin(t_mem *mem, t_cmdlst *lst)
{
	if (ft_strcmp(lst->command, "cd") == 0
		|| ft_strcmp(lst->command, "exit") == 0
		|| ft_strcmp(lst->command, "export") == 0
		|| ft_strcmp(lst->command, "unset") == 0)
		return (0);
	if (ft_strcmp(lst->command, "pwd") == 0)
		ft_pwd(mem);
	if (ft_strcmp(lst->command, "env") == 0)
		ft_env(mem);
	if (ft_strcmp(lst->command, "echo") == 0)
		ft_echo(lst->args, mem);
	return (1);
}

/*
	Minishell built-in function (pwd)
	Print the PWD from the env if found
*/
int	ft_pwd(t_mem *mem)
{
	char	*pwd;

	pwd = my_getenv(mem->my_env, "PWD");
	if (pwd)
	{
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		mem->exit_statue = 0;
		free(pwd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		ft_printf("\033[91m%s\033[0m\n",
			"minishell: pwd does not work if PWD is unset");
		mem->exit_statue = 1;
		exit(EXIT_FAILURE);
	}
}

int	ft_env(t_mem *mem)
{
	int	x;

	x = -1;
	while (mem->my_env[++x])
		ft_printf("%s\n", mem->my_env[x]);
	mem->exit_statue = 0;
	return (1);
}

int	ft_echo(char **s, t_mem *mem)
{
	int		i;
	int		has_n;

	i = 0;
	has_n = 0;
	if (s[1] && s[1][0] == '-')
		if (contains_only(s[1] + 1, 'n'))
			has_n = 1;
	if (has_n)
		i++;
	while (s[++i])
	{
		if ((!has_n && i > 1) || (has_n && i > 2))
			ft_putchar_fd(' ', 1);
		echo_arg(s[i]);
	}
	if (!has_n || strarr_len(s) == 1)
		ft_putchar_fd('\n', 1);
	mem->exit_statue = 0;
	return (1);
}

/*
	Return 1 if the given str contains only the given char
*/
int	contains_only(char *str, char c)
{
	int	cur;

	cur = -1;
	while (str[++cur])
		if (str[cur] != c)
			return (0);
	return (1);
}

void	echo_arg(char *str)
{
	int	cur;

	cur = -1;
	while (str[++cur])
	{
		if (ft_strncmp(str + cur, "\\n", 2) == 0)
		{
			ft_putchar_fd('\n', 1);
			cur++;
		}
		else
			ft_putchar_fd(str[cur], 1);
	}
}

/*
	Print the error corresponding to the given error code
*/
static int	print_error(int error_code, char *content)
{
	if (error_code == 14)
	{
		ft_printf("\033[91m%s '%s'\033[0m\n",
			"minishell: command not found:", content);
	}
	else if (error_code == 13)
	{
		ft_printf("\033[91m%s '%s'\033[0m\n",
			"minishell: permission denied:", content);
	}
	return (1);
}

// ================================================ Execute : Execute next : Built in cd export unset

/*
	exec builtin cmd that change env
*/
int	do_builtin(t_cmdlst *lst, t_mem *mem)
{
	if (ft_strcmp(lst->command, "cd") == 0 && lst->todo_next != 1)
		ft_cd(lst->args, mem);
	if (ft_strcmp(lst->command, "export") == 0 && lst->todo_next != 1)
		if (!ft_exports(mem, lst->args))
			return (0);
	if (ft_strcmp(lst->command, "unset") == 0 && lst->todo_next != 1)
		if (!ft_unsets(mem, lst->args))
			return (0);
	return (1);
}

// ================================================ Execute : Execute next : Built in cd export unset : CD

void	ft_cd(char **cmd, t_mem *mem)
{
	if (change_dir(cmd, mem))
	{
		mem->my_env = change_my_env(cmd, mem->my_env);
		mem->exit_statue = 0;
	}
	else
		mem->exit_statue = 1;
}

/*
	if can't change directory (chdir return -1), exit statut = 1
	else change env (pwd, oldpwd), exit statut = 0
*/
int	change_dir(char **cmd, t_mem *mem)
{
	char	*home;

	home = my_getenv(mem->my_env, "HOME");
	if (!cmd[1])
	{
		if (chdir(home) == -1)
		{
			write(1, strerror(errno), ft_strlen(strerror(errno)));
			write(1, "\n", 1);
			free(home);
			return (0);
		}
	}
	else if (chdir(cmd[1]) == -1)
	{
		write(1, strerror(errno), ft_strlen(strerror(errno)));
		write(1, "\n", 1);
		free(home);
		return (0);
	}
	free(home);
	return (1);
}

/*
	change env (oldpwd and pwd) depending on if it's absolute or relativ path
*/
char	**change_my_env(char **cmd, char **my_env)
{
	char	*pwd;

	pwd = my_getenv(my_env, "PWD");
	if (!pwd)
	{
		ft_printf("\033[91m%s\033[0m\n",
			"minishell: cd does not work if PWD is unset");
		return (my_env);
	}
	if (!cmd[1])
	{
		my_env = change_pwd_home(my_env);
		my_env = change_oldpwd(pwd, my_env);
	}
	else if (cmd[1][0] == '/')
	{
		my_env = change_pwd_absolute(cmd[1], my_env);
		my_env = change_oldpwd(pwd, my_env);
	}
	else
		my_env = change_pwd_relativ(cmd[1], my_env);
	free(pwd);
	return (my_env);
}

char	**change_pwd_home(char **my_env)
{
	char	*new_pwd;
	int		pwd_index;
	char	*home;

	home = my_getenv(my_env, "HOME");
	pwd_index = my_env_index_elem(my_env, "PWD");
	new_pwd = ft_strjoin("PWD=", home);
	free(my_env[pwd_index]);
	my_env[pwd_index] = ft_strdup(new_pwd);
	free(new_pwd);
	free(home);
	return (my_env);
}

/*
	return index of given elem in env
	return -1 if not found
*/
int	my_env_index_elem(char **my_env, char *elem)
{
	int	path_len;
	int	index;

	path_len = ft_strlen(elem);
	index = 0;
	while (my_env[index])
	{
		if (ft_strncmp(elem, my_env[index], path_len) == 0)
			break ;
		index++;
	}
	if (index == strarr_len(my_env))
		return (-1);
	return (index);
}

/*
	change path of oldpwd in env
	return env with new oldpwd
*/
char	**change_oldpwd(char *pwd, char **my_env)
{
	char	*new_old_pwd;
	int		old_env_index;

	old_env_index = my_env_index_elem(my_env, "OLDPWD");
	new_old_pwd = ft_strjoin("OLDPWD=", pwd);
	free(my_env[old_env_index]);
	my_env[old_env_index] = ft_strdup(new_old_pwd);
	free(new_old_pwd);
	return (my_env);
}

/*
	change path of pwd in env for absolute path
*/
char	**change_pwd_absolute(char *path, char **my_env)
{
	int		i;
	int		pwd_index;
	char	**temp;
	char	*new_pwd;

	i = 0;
	temp = (char **)malloc(sizeof(char *) * (tab_2d_len(my_env) + 1));
	if (!temp)
		return (NULL);
	pwd_index = my_env_index_elem(my_env, "PWD");
	new_pwd = ft_strjoin("PWD=", path);
	while (my_env[i])
	{
		if (i == pwd_index)
			temp[i] = ft_strdup(new_pwd);
		else
			temp[i] = ft_strdup(my_env[i]);
		i++;
	}
	temp[i] = NULL;
	free_tab_2d(my_env);
	free(new_pwd);
	return (temp);
}

int	tab_2d_len(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

/*
	change path of pwd in env for relativ path
*/
char	**change_pwd_relativ(char *path, char **my_env)
{
	char	**temp;
	char	**temp2;
	char	*pwd;
	int		i;

	i = 0;
	pwd = my_getenv(my_env, "PWD");
	temp = ft_split(path, '/');
	temp2 = ft_split(pwd, '/');
	while (temp[i])
	{
		if (ft_strcmp(temp[i], "..") == 0)
			temp2 = supp_last_elem_tab2d(temp2);
		else if (ft_strcmp(temp[i], ".") != 0)
			temp2 = append_tab_2d(temp2, temp[i]);
		i++;
	}
	my_env = change_pwd(temp2, my_env);
	my_env = change_oldpwd(pwd, my_env);
	free_tab_2d(temp);
	free_tab_2d(temp2);
	free(pwd);
	return (my_env);
}

char	**supp_last_elem_tab2d(char **tab)
{
	char	**new;
	int		len;
	int		i;

	i = 0;
	len = tab_2d_len(tab);
	new = (char **)malloc(sizeof(char *) * (tab_2d_len(tab)));
	if (!new)
		return (NULL);
	while (i < len - 1)
	{
		new[i] = ft_strdup(tab[i]);
		i++;
	}
	new[i] = NULL;
	free_tab_2d(tab);
	return (new);
}

char	**append_tab_2d(char **tab, char *s)
{
	int		i;
	char	**new;

	i = 0;
	new = (char **)malloc(sizeof(char *) * (tab_2d_len(tab) + 2));
	if (!new)
		return (NULL);
	while (tab[i])
	{
		new[i] = ft_strdup(tab[i]);
		i++;
	}
	new[i] = ft_strdup(s);
	i++;
	new[i] = NULL;
	free_tab_2d(tab);
	return (new);
}

/*
	change path of pwd in env
	return env with new pwd
*/
char	**change_pwd(char **temp2, char **my_env)
{
	int		pwd_place;
	char	*new_pwd;
	char	**temp;
	int		i;

	i = 0;
	pwd_place = my_env_index_elem(my_env, "PWD");
	temp = (char **)malloc(sizeof(char *) * (tab_2d_len(my_env) + 1));
	if (!temp)
		return (NULL);
	pwd_place = my_env_index_elem(my_env, "PWD");
	new_pwd = concat_path(temp2, "PWD=");
	while (my_env[i])
	{
		if (i == pwd_place)
			temp[i] = ft_strdup(new_pwd);
		else
			temp[i] = ft_strdup(my_env[i]);
		i++;
	}
	temp[i] = NULL;
	free_tab_2d(my_env);
	free(new_pwd);
	return (temp);
}

/*
	return new pwd path
*/
char	*concat_path(char **tab, char *str)
{
	char	*s;
	char	*temp;
	int		i;

	i = 0;
	s = ft_strdup(str);
	temp = ft_strdup(s);
	free(s);
	while (tab[i])
	{
		s = ft_strjoin(temp, "/");
		free(temp);
		temp = ft_strdup(s);
		free(s);
		s = ft_strjoin(temp, tab[i]);
		free(temp);
		temp = ft_strdup(s);
		free(s);
		i++;
	}
	s = ft_strdup(temp);
	free(temp);
	return (s);
}

// ================================================ Execute : Execute next : Built in cd export unset : EXPORTS

/*
   Run the function ft_export on every args
   Return 0 on malloc error (some env var may have been set)
*/
int	ft_exports(t_mem *mem, char **args)
{
	int	i;
	int	command_return;

	i = 1;
	mem->exit_statue = 0;
	while (args[i])
	{
		command_return = ft_export(mem, args[i]);
		if (command_return == 0)
			return (0);
		else if (command_return == 2)
			mem->exit_statue = 1;
		i++;
	}
	return (1);
}

/*
	Parse the current string (an arg) and add it to mem->my_env
	Return 0 on malloc error
	Return 1 on succeed
	Return 2 on command error (bad syntax, etc.)
*/
int	ft_export(t_mem *mem, char *s)
{
	char	**new;
	char	**temp;

	if (check_format(s))
		return (2);
	temp = ft_split(s, '=');
	if (!temp)
		return (0);
	if (is_in_env(mem->my_env, temp[0]))
	{
		new = ft_unset(mem, temp[0]);
		if (!temp)
			return (strarr_free(temp));
		strarr_free(mem->my_env);
		mem->my_env = new;
	}
	strarr_free(temp);
	if (!strarr_append(&(mem->my_env), s))
		return (0);
	return (1);
}

/*
	Return 1 on syntax error, 0 if there is no error
	Error :	The env name starts with a number
	The env name contains other chars than [A-Z][a-z][0-9]_
	The arg doesn't contain an equal sign
	Display an error with ft_printf when the error is not the missing equal sign

	E.g.:	1name=oliv
	na-me=oliv
	name = oliv
*/
int	check_format(char	*arg)
{
	int	cur;
	int	has_equal;

	if (!ft_isalpha(arg[0]))
		return (print_error(1, &arg[0]));
	cur = 0;
	has_equal = 0;
	while (arg[cur])
	{
		if (arg[cur] == '=')
			has_equal = 1;
		else if (!ft_isalnum(arg[cur]) && arg[cur] != '_')
			if (!has_equal)
				return (print_error(2, &arg[cur]));
		cur++;
	}
	if (!has_equal)
		return (1);
	return (0);
}

/*
	Print the error corresponding to the given error code
*/
static int	print_error(int error_code, char *content)
{
	if (error_code == 1)
	{
		ft_printf("\033[91m%s '%c'\033[0m\n",
			"export: name cannot start with", *content);
	}
	else if (error_code == 2)
	{
		ft_printf("\033[91m%s '%c'\033[0m\n",
			"export: not valid in this context:", *content);
	}
	return (1);
}

int	is_in_env(char **my_env, char *s)
{
	int		i;
	char	**temp;

	i = 0;
	while (my_env[i])
	{
		temp = ft_split(my_env[i], '=');
		if (ft_strcmp(temp[0], s) == 0)
			break ;
		free_tab_2d(temp);
		temp = NULL;
		i++;
	}
	if (temp)
		free_tab_2d(temp);
	if (i == tab_2d_len(my_env))
		return (0);
	return (1);
}

/*
	if var isnt find return env without refreshing it
	else return a refresh env without this var
*/
char	**ft_unset(t_mem *mem, char *var)
{
	char	**new;
	char	**temp;
	int		i;
	int		j;

	i = -1;
	j = 0;
	new = (char **)malloc(strarr_len(mem->my_env) * sizeof(char *));
	if (!new)
		return (NULL);
	while (mem->my_env[++i])
	{
		temp = ft_split(mem->my_env[i], '=');
		if (!temp)
			return (nil(free_array_n(new, j)));
		if (strcmp_len(temp[0], var))
		{
			new[j++] = ft_strdup(mem->my_env[i]);
			if (!new[j - 1])
				return (nil(free_array_n(new, j - 1) + strarr_free(temp)));
		}
		strarr_free(temp);
	}
	new[j] = 0;
	return (new);
}

/*
	Return a NULL (void *)
	The given int is ignored
*/
void	*nil(int i)
{
	(void)i;
	return (NULL);
}

// ================================================ Execute : Execute next : Built in cd export unset : UNSET

int	ft_unsets(t_mem *mem, char **args)
{
	int		i;
	char	**temp;

	mem->exit_statue = 0;
	i = 0;
	while (args[++i])
	{
		if (!is_in_env(mem->my_env, args[i]))
		{
			mem->exit_statue++;
			continue ;
		}
		temp = ft_unset(mem, args[i]);
		if (!temp)
			return (0);
		strarr_free(mem->my_env);
		mem->my_env = temp;
	}
	return (1);
}

// ================================================ Execute : Execute next : Delete Heredoc

/*
	if there was a << in command
	delete tmpfile
*/
void	delete_tpmfile(t_mem *mem)
{
	char	**argv;
	pid_t	pid;

	argv = malloc(sizeof(char *) * 3);
	argv[0] = "/bin/rm";
	argv[1] = mem->tmpfile;
	argv[2] = NULL;
	pid = fork();
	if (pid == 0)
		execve("/bin/rm", argv, NULL);
}

// ================================================ Execute : Save exit status

/*
   save last exit statue for && || $? behaviors
   return 1 if :
   -> exit statut = 0 and to_do_next is ||
   -> exit statut != 0 and to_do_next is &&
   if to_do_next is || or && reset and init, cuz it's like doing a new cmd
*/
int	save_last_exit_statue(t_mem *mem, t_cmdlst *lst)
{
	mem->last_cmd_exit_statue = mem->exit_statue;
	if (mem->exit_statue == 0 && lst->todo_next == 3)
		return (1);
	if (mem->exit_statue != 0 && lst->todo_next == 2)
		return (1);
	if (lst->todo_next == 2 || lst->todo_next == 3)
	{
		reset_exec(mem);
		init_exec(mem, lst);
	}
	return (0);
}
