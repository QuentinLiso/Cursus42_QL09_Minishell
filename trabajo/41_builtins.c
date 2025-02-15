#include "minishell.h"

bool	is_builtin(char *s)
{
	if (ft_strcmp(s, "echo") == 0)
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

t_error	b_in(char *s, char **args, t_mnsh *mnsh, char ***env)
{
	if (ft_strcmp(s, "echo") == 0)
		return (mnsh_echo(args, mnsh));
	else if (ft_strcmp(s, "cd") == 0)
		return (mnsh_cd(args, mnsh));
	else if (ft_strcmp(s, "pwd") == 0)
		return (mnsh_pwd(mnsh));
	else if (ft_strcmp(s, "export") == 0)
		return (mnsh_export(args, mnsh));
	else if (ft_strcmp(s, "unset") == 0)
		return (mnsh_unset(args, mnsh, env));
	else if (ft_strcmp(s, "env") == 0)
		return (mnsh_env(mnsh));
	else if (ft_strcmp(s, "exit") == 0)
		return (mnsh_exit(args, mnsh));
	return (false);	
}

//============================================================================

t_error	mnsh_echo(char **args, t_mnsh *mnsh)
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
	mnsh->last_exit_status = 0;
	return (ERR_NOERR);
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

t_error	mnsh_env(t_mnsh *mnsh)
{
	char	**env;
	int		i;

	env = mnsh->env_mnsh;
	if (!env || !env[0])
		return (mnsh_perror(ERR_ENV));
	i = -1;
	while (env[++i])
		if (ft_str_contain(env[i], '='))
			ft_putendl_fd(env[i], STDOUT_FILENO);
	mnsh->last_exit_status = 0;
	return (ERR_NOERR);
}

//============================================================================

t_error	mnsh_pwd(t_mnsh *mnsh)
{
	char	*pwd;

	pwd = NULL;
	pwd = getcwd(pwd, 0);
	if (pwd)
	{
		ft_putendl_fd(pwd, STDOUT_FILENO);
		mnsh->last_exit_status = 0;
		free(pwd);
		return (ERR_NOERR);
	}
	else
	{
		mnsh->last_exit_status = 1;
		return (mnsh_perror(ERR_ENV));
	}
}

//============================================================================

t_error	mnsh_cd(char **args, t_mnsh *mnsh)
{
	char	*oldpwd;
	char	cwd[512];

	if (ft_strarrlen(args) > 2)
		return (ERR_ARGS);
	if (args[1] == NULL)
	{
		args[1] = ft_get_env_var(mnsh->env_mnsh, "HOME");
		if (!args[1])
			return (mnsh_perror(ERR_ENV));
	}
	if (chdir(args[1]) != 0)
		return (mnsh_perror(ERR_CD));
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (mnsh_perror(ERR_CD));
	oldpwd = ft_get_env_var(mnsh->env_mnsh, "PWD");
	if (!oldpwd)
			return (mnsh_perror(ERR_ENV));
	else
		ft_reset_env_var(&mnsh->env_mnsh, "OLDPWD", oldpwd);
	ft_reset_env_var(&mnsh->env_mnsh, "PWD", cwd);
	free(oldpwd);
	return (ERR_NOERR);
}

//============================================================================

t_error	mnsh_export(char **args, t_mnsh *mnsh)
{
	int	i;
	int	j;

	mnsh->last_exit_status = 0;
	if (args[1] == NULL)
		return (ft_print_export_var(mnsh->env_mnsh));
	else
	{
		i = 0;
		while (args[++i])
		{
			j = 0;
			if (export_check_specials(args[i], &j) == ERR_NOERR)
				handle_export_var(&mnsh->env_mnsh, args[i], &j);
			else
			{
				mnsh_perror(ERR_ARGS);
				mnsh->last_exit_status = 1;
			}
		}
	}
	return (ERR_NOERR);
}

t_error	ft_print_export_var(char **env)
{
	int	i;
	int	j;

	i = 0;
	if (!env || !env[0])
		return (ERR_ENV);
	while (env[i])
	{
		j = 0;
		ft_putstr_fd("export ", STDOUT_FILENO);
		ft_print_substr_before_char(env[i], '=', &j);
		if (env[i][j] == '=')
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_print_substr_after_char(env[i], &j);
			ft_putstr_fd("\"", STDOUT_FILENO);	
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (ERR_NOERR);
}

void	ft_print_substr_before_char(char *s, char c, int *j)
{
	if (!s || !s[*j])
		return ;
	while (s[*j] && s[*j] != c)
		ft_putchar_fd(s[(*j)++], STDOUT_FILENO);
}

void	ft_print_substr_after_char(char *s, int *j)
{
	if (!s || !s[*j])
		return ;
	while (s[++(*j)])
		ft_putchar_fd(s[*j], STDOUT_FILENO);
}

t_error	export_check_specials(char *s, int *j)
{
	if (!s || !s[0])
		return (mnsh_perror(ERR_ARGS));
	while (s[*j] && (ft_isalnum(s[*j]) || s[*j] == '_'))
		(*j)++;
	if (s[*j] && s[*j] != '=')
		return (ERR_ARGS);
	return (ERR_NOERR);
}

t_error	handle_export_var(char ***env, char *arg, int *j)
{
	char	*value;
	char	*var;
	int		i;
	t_error	status;

	if (arg[*j] == '=')
		value = &arg[*j + 1];
	else
		value = NULL;
	var = ft_strndup(arg, *j);
	if (!var)
		return (perror_malloc("handle_export_var"));
	i = ft_get_env_var_index(*env, var);
	if (i == -1)
	{
		status = ft_new_env_var(env, var, value);
	}
	else
		status = ft_reset_env_var_index(env, var, value, i);
	free(var);
	return (status);
}

//============================================================================

t_error	mnsh_unset(char **args, t_mnsh *mnsh, char ***env)
{
	char	**new_env;
	int		i;
	int		j;

	if (!args[1])
		return (ERR_NOERR);
	new_env = ft_calloc(unset_newenvlen(env, args) + 1, sizeof(char *));
	if (!new_env)
		return (perror_malloc("mnsh_unset"));
	i = -1;
	j = 0;
	while ((*env)[++i])
	{
		if (!env_var_is_in_args((*env)[i], &args[1]))
		{
			new_env[j++] = ft_strdup((*env)[i]);
				if (!new_env[j - 1])
					return (perror_malloc("mnsh_unset"));
		}
	}
	new_env[j] =  NULL;
	ft_free_strarray(env);
	*env = new_env;
	mnsh->last_exit_status = 0;
	return (ERR_NOERR);
}

size_t	unset_newenvlen(char ***env, char **args)
{
	size_t	len;
	int		i;

	len = 0;
	i = -1;
	while ((*env)[++i])
		if (!env_var_is_in_args((*env)[i], args))
			len++;
	return (len);
}

bool	env_var_is_in_args(char *var, char **args)
{
	char	*buffer;

	buffer = ft_strndup(var, ft_strlenchar(var, '='));
	if (str_is_in_arr(buffer, args))
	{
		free(buffer);
		return (true);
	}
	free(buffer);
	return (false);
}

bool	str_is_in_arr(char *s, char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		if (ft_strcmp(s, arr[i]) == 0)
			return (true);
	return (false);
}

//============================================================================

t_error	mnsh_exit(char **args, t_mnsh *mnsh)
{
	unsigned int	exit_code;

	if (ft_strarrlen(args) > 2)
		return (mnsh_perror(ERR_ARGS));
	if (args[1])
		exit_code = ft_atoi(args[1]);
	else
		exit_code = 0;
	ft_free_all_tok(&mnsh->tokis);
	ft_free_ast(mnsh->node);
	ft_free_str(&mnsh->prompt);
	ft_free_strarray(&mnsh->env_mnsh);
	ft_free_strarray(&mnsh->paths);
	rl_clear_history();
	load_message(17, "☑️  EXIT SUCCESSFUL ☑️\tSee you later :)", 120000);
	exit(exit_code);
	return (ERR_NOERR);
}