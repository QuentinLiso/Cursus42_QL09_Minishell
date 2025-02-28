#include "minishell.h"

bool	is_builtin(char *s)
{
	if (!s)
		return (false);
	else if (ft_strcmp(s, "echo") == 0)
		return (true);
	else if (ft_strcmp(s, "cd") == 0)
		return (true);
	else if (ft_strcmp(s, "pwd") == 0)
		return (true);
	else if (ft_strcmp(s, "export") == 0)
		return (true);
	else if (ft_strcmp(s, "unset") == 0)
		return (true);
	else if (ft_strcmp(s, "env") == 0)
		return (true);
	else if (ft_strcmp(s, "exit") == 0)
		return (true);
	return (false);	
}

int		exec_ast_cmd_builtin(char **args, t_mnsh *mnsh)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (mnsh_echo(args));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (mnsh_cd(args, mnsh));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (mnsh_pwd());
	else if (ft_strcmp(args[0], "export") == 0)
		return (mnsh_export(args, mnsh));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (mnsh_unset(args, mnsh));
	else if (ft_strcmp(args[0], "env") == 0)
		return (mnsh_env(mnsh));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (mnsh_exit(args, mnsh));
	return (0);	
}

//============================================================================


int	mnsh_echo(char **args)
{
	int		i;
	bool	option;

	if (args[1] == NULL)
		printf("\n");
	else
	{
		i = 0;
		option = false;
		while (args[++i])
			if (!is_echo_option_valid(args[i]))
				break;
			else
				option = true;
		print_strarray_raw(&args[i], ' ');
		if (!option)
			ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}

bool	is_echo_option_valid(char *arg)
{
	int	i;

	if (ft_strncmp(arg, "-n", 2) == 0)
	{
		i = 1;
		while (arg[++i])
			if (arg[i] != 'n')
				return (false);
		return (true);
	}
	return (false);
}

//============================================================================

int	mnsh_env(t_mnsh *mnsh)
{
	t_list	*iterator;
	t_var	*var;

	iterator = mnsh->env_mnsh_lst;
	while (iterator)
	{
		var = (t_var *)iterator->content;
		if (var->value)
		{
			ft_putstr_fd(var->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(var->value, STDOUT_FILENO);
		}
		iterator = iterator->next;
	}
	return (0);
}

//============================================================================

int	mnsh_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		ft_putendl_fd(pwd, STDOUT_FILENO);
		free_str(&pwd);
		return (0);
	}
	else
		return (perror_mnsh(errno_to_exit(errno), 1, strerror(errno)));
}

//============================================================================

int	mnsh_cd(char **args, t_mnsh *mnsh)
{
	int		status;
	char	*cwd;
	char	*target;

	if (ft_strarrlen(args) > 2)
		return (perror_mnsh(1, 1, "too many arguments"));
	if (set_target(args, &target, mnsh))
		return (1);
	status = set_cwd(&cwd);
	if (chdir(target))
	{
		free_str(&cwd);
		return (perror_mnsh(1, 1, strerror(errno)));
	}
	if (status)
		return (status);
	status = update_pwd_oldpwd(cwd, mnsh);
	free_str(&cwd);
	if (status)
		return (status);
	return (0);
}

int	set_target(char **args, char **target, t_mnsh *mnsh)
{
	if (!args[1])
	{
		*target = get_env_var(mnsh->env_mnsh_lst, "HOME");
		if (!*target)
			return (perror_mnsh(1, 1, "HOME env var not set"));
	}
	else
		*target = args[1];
	return (0);
}

int	set_cwd(char **cwd)
{
	*cwd = getcwd(NULL, 0);
	if (!*cwd)
		return (perror_mnsh(errno_to_exit(errno), 1, strerror(errno)));
	return (0);
}

int	update_pwd_oldpwd(char *cwd, t_mnsh *mnsh)
{
	char	*newwd;
	int		status_cwd;
	int		status_newwd;

	newwd = getcwd(NULL, 0);
	if (!newwd)
		return (perror_mnsh(errno_to_exit(errno), 1, strerror(errno)));
	status_newwd = edit_env_var(&mnsh->env_mnsh_lst, "PWD", newwd);
	status_cwd = edit_env_var(&mnsh->env_mnsh_lst, "OLDPWD", cwd);
	if (status_newwd)
		perror_mnsh(errno_to_exit(status_newwd), 1, "Could not update PWD");
	if (status_cwd)
		perror_mnsh(errno_to_exit(status_cwd), 1, "Could not update OLDPWD");
	free_str(&newwd);
	if (status_newwd)
		return (errno_to_exit(status_newwd));
	if (status_cwd)
		return (errno_to_exit(status_cwd));
	return (0);
}

//============================================================================

int	mnsh_export(char **args, t_mnsh *mnsh)
{
	int	i;
	int	status;

	status = 0;
	if (args[1] == NULL)
		return (print_export_var(mnsh->env_mnsh_lst));
	else
	{
		i = 0;
		while (args[++i])
			status = handle_export_var(args[i], mnsh);
	}
	return (status);
}

int	print_export_var(t_list *env)
{
	t_var	*var;

	if (!env)
		return (perror_mnsh(1, 1, "ENV not available"));
	while (env)
	{
		var = (t_var *)env->content;
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(var->key, STDOUT_FILENO);
		if (var->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(var->value, STDOUT_FILENO);
			ft_putchar_fd('"', STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		env = env->next;
	}
	return (0);
}

int	handle_export_var(char *arg, t_mnsh *mnsh)
{
	size_t	i;
	char	*key;
	char	*value;
	int		status;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (perror_mnsh(1, 3, "export", arg, "not a valid identifier"));
	i = 0;
	while(arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (perror_mnsh(1, 3, "export", arg, "not a valid identifier"));
		i++;
	}
	status = set_export_key_value(arg, i, &key, &value);
	if (status)
		return (status);
	status = update_export_var(key, value, mnsh);
	free_str(&key);
	free_str(&value);
	return (status);
}

int	set_export_key_value(char *arg, int i, char **key, char **value)
{
	*key = ft_substr(arg, 0, i);
	if (!*key)
		return (perror_mnsh(ENOMEM, 1, "malloc err in set export key val"));
	if (!arg[i])
		*value = NULL;
	else
	{
		*value = ft_strdup(&arg[i + 1]);
		if (!*value)
		{
			free_str(key);
			return (perror_mnsh(ENOMEM, 1, "malloc err in set export key val"));
		}
	}
	return (0);
}

int	update_export_var(char *key, char *value, t_mnsh *mnsh)
{
	char	*env_var_val;

	env_var_val = get_env_var(mnsh->env_mnsh_lst, key);
	if (!value)
	{
		if (!env_var_val)
			return (add_env_var(&mnsh->env_mnsh_lst, key, value));
		return (0);
	}
	else
		return (edit_env_var(&mnsh->env_mnsh_lst, key, value));
}

//============================================================================

int	mnsh_unset(char **args, t_mnsh *mnsh)
{
	int		i;

	i = 0;
	while (args[++i])
		del_node(&mnsh->env_mnsh_lst, args[i]);
	return (0);
}

void	del_node(t_list **list, char *key)
{
	t_list	*iter;
	t_list	*prev;
	t_var	*var;

	if (!list || !*list || !key)
		return ;
	iter = *list;
	prev = NULL;
	while (iter)
	{
		var = (t_var *)iter->content;
		if (!ft_strcmp(var->key, key))
			break;
		prev = iter;
		iter = iter->next;
	}
	if (!iter)
		return ;
	if (!prev)
		*list = iter->next;
	else
		prev->next = iter->next;
	ft_lstdelone(iter, &free_env_var);
}


// //============================================================================

int	mnsh_exit(char **args, t_mnsh *mnsh)
{
	long long		exit_code;
	bool			is_llong_num;

	if (args[1] == NULL)
		exit_code = mnsh->last_exit_status;
	else 
	{
		is_llong_num = strtoll_isnum(args[1], &exit_code);
		if (!is_llong_num)
			exit_code = perror_mnsh(2, 3, "exit", args[1],
				"numeric argument required");				
		else if (is_llong_num && args[2])
			return (perror_mnsh(errno_to_exit(E2BIG), 2, "exit",
				"too many arguments"));
	}
	free_all_mnsh(mnsh);
	// load_message(17, "☑️  EXIT SUCCESSFUL ☑️\tSee you later :)", 120000);
	mnsh->last_exit_status = exit_code % 256;
	exit(exit_code % 256);
	return (0);
}

bool	strtoll_isnum(char *str, long long *n)
{
	int	sign;
	int	digit;

	*n = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
		sign = ',' - *str++;
	if (!*str)
		return (false);
	while (*str >= '0' && *str <= '9')
	{
		digit = *str++ - '0';
		if ((sign == 1 && (*n > (LLONG_MAX - digit) / 10)) ||
			(sign == -1 && (*n * sign < (LLONG_MIN + digit) / 10)))
			return (false);
		*n = *n * 10 + digit;
	}
	if (*str)
		return (false);
	*n *= sign;
	return (true);
}

