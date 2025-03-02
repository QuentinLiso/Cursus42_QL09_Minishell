/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   50_builtins_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:19:32 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 16:19:54 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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