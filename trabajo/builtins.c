#include "minishell.h"

bool	check_builtin(char *s)
{
	if (ft_strcmp(s, "echo"))
		return (true);
	else if (ft_strcmp(s, "cd"))
		return (true);
	else if (ft_strcmp(s, "pwd"))
		return (true);
	else if (ft_strcmp(s, "export"))
		return (true);
	else if (ft_strcmp(s, "unset"))
		return (true);
	else if (ft_strcmp(s, "env"))
		return (true);
	else if (ft_strcmp(s, "exit"))
		return (true);
	return (false);	
}

void	mnsh_echo(char **args, int *mnsh_status)
{
	if (args[1] == NULL)
		printf("\n");
	else if (ft_strcmp(args[1], "-n") == 0)
	{
		if (args[2])
			print_strarray_raw(&args[2], ' ');
	}
	else
	{
		print_strarray_raw(&args[1], ' ');
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	*mnsh_status = 0;
}

void	mnsh_env(t_mnsh *mnsh)
{
	char	**env;
	int		i;

	env = mnsh->env_mnsh;
	i = -1;
	while (env[++i])
		if (ft_str_contain(env[i], '='))
			ft_putendl_fd(env[i], STDOUT_FILENO);
	mnsh->last_exit_status = 0;
}

void	mnsh_pwd(t_mnsh *mnsh)
{
	char	*pwd;

	pwd = ft_get_env_var(mnsh->env_mnsh, "PWD");
	if (pwd)
	{
		ft_putendl_fd(pwd, STDOUT_FILENO);
		mnsh->last_exit_status = 0;
		free(pwd);
	}
	else
	{
		ft_printf("Error PWD is not set\n");
		mnsh->last_exit_status = 1;
	}
}

t_error	mnsh_cd(char *path, t_mnsh *mnsh)
{
	char	*oldpwd;
	char	cwd[512];

	oldpwd = ft_get_env_var(mnsh->env_mnsh, "PWD");
	if (path == NULL || *path == '\0')
	{
		path = ft_get_env_var(mnsh->env_mnsh, "HOME");
		if (!path)
			return (mnsh_perror(ERR_ENV));
	}
	if (chdir(path) != 0)
		return (mnsh_perror(ERR_CD));
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (mnsh_perror(ERR_CD));
	if (oldpwd)
		ft_reset_env_var(&mnsh->env_mnsh, "OLDPWD", oldpwd);
	ft_reset_env_var(&mnsh->env_mnsh, "PWD", cwd);
	return (ERR_NOERR);
}

void	mnsh_export(char **args, t_mnsh *mnsh)
{
	int	i;
	int	j;

	mnsh->last_exit_status = 0;
	if (args[1] == NULL)
		ft_print_export_var(mnsh->env_mnsh);
	else
	{
		i = 0;
		while (args[++i])
		{
			j = 0;
			if (export_check_specials(args[i], &j) == ERR_NOERR)
				handle_export_var(&mnsh->env_mnsh, args[i], &j);
			else
				mnsh->last_exit_status = 1;
		}
	}
}

void	ft_print_export_var(char **env)
{
	int	i;
	int	j;

	i = 0;
	if (!env || !env[0])
		return ;
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
		return (ERR_ARGS);
	while (s[*j] && (ft_isalnum(s[*j]) || s[*j] == '_'))
		(*j)++;
	if (s[*j] && s[*j] != '=')
		return (ERR_ARGS);
	return (ERR_NOERR);
}

void	handle_export_var(char ***env, char *arg, int *j)
{
	char	*value;
	char	*var;
	int		i;

	if (arg[*j] == '=')
		value = &arg[*j + 1];
	else
		value = NULL;
	var = ft_strndup(arg, *j);
	i = ft_get_env_var_index(*env, var);
	if (i == -1)
		ft_new_env_var(env, var, value);
	else
	{
		if (value)
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin_multi(3, var, "=", value);
		}
	}
	free(var);
}

t_error	mnsh_unset(char ***env, char **args, t_mnsh *mnsh)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = ft_calloc(unset_newenvlen(env, args), sizeof(char *));
	if (!new_env)
		return (perror_malloc("mnsh_unset"));
	i = -1;
	j = 0;
	while ((*env)[++i])
	{
		if (!env_var_is_in_args((*env)[i], args))
		{
			new_env[j++] = ft_strdup((*env)[i]);
			free((*env)[i]);
				if (!new_env[j - 1])
					return (perror_malloc("mnsh_unset"));
		}
	}
	new_env[j] =  NULL;
	free(*env);
	(*env) = new_env;
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
		return (true);
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

