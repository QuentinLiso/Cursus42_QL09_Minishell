/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:11:43 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/04 12:21:20 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_ast_cmd_ext_parent(pid_t pid, t_mnsh *mnsh)
{
	int	status;

	set_sigaction(&mnsh->sa_sigint, SIGINT, SIG_IGN, 0);
	waitpid(pid, &status, 0);
	set_sigaction(&mnsh->sa_sigint, SIGINT, h_sigint_loop, 0);
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		if (status == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (status == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + status);
	}
	return (WEXITSTATUS(status));
}

int	check_and_execute_cmd(char **args, t_mnsh *mnsh)
{
	char		*execfile;
	int			status;

	if (!args || !args[0])
		return (0);
	status = set_execfile(&execfile, args, mnsh);
	if (status == ENOMEM)
		return (status);
	status = check_execfile(execfile, args);
	if (status)
	{
		free_str(&execfile);
		return (status);
	}
	return (ft_execve(&execfile, args, mnsh));
}

int	set_execfile(char **execfile, char **args, t_mnsh *mnsh)
{
	char	**paths;
	int		status;

	*execfile = NULL;
	if (*args[0] && !ft_strchr(args[0], '/'))
	{
		status = set_execfile_paths(mnsh->env_mnsh_lst, &paths);
		if (status)
			return (status);
		status = get_cmd_path(execfile, args[0], paths);
		free_strarray(&paths);
		return (status);
	}
	else
	{
		*execfile = ft_strdup(args[0]);
		if (!*execfile)
			return (perror_mnsh(ENOMEM, 1, "err malloc in set exec file"));
	}
	return (0);
}

int	set_execfile_paths(t_list *env_mnsh_list, char ***paths)
{
	char	*path;
	int		i;

	path = get_env_var(env_mnsh_list, "PATH");
	if (!path)
		return (1);
	*paths = ft_split(path, ':');
	if (!*paths)
		return (ENOMEM);
	i = -1;
	while ((*paths)[++i])
	{
		if ((*paths)[i][ft_strlen((*paths)[i]) - 1] != '/')
		{
			(*paths)[i] = ft_strappend_mnsh((*paths)[i], "/");
			if (!(*paths)[i])
			{
				free_strarray(paths);
				return (ENOMEM);
			}
		}
	}
	return (0);
}

int	check_execfile(char *execfile, char **args)
{
	struct stat	st;

	if (!execfile || *execfile == '\0')
		return (perror_mnsh(127, 2, args[0], "command not found"));
	if (stat(execfile, &st) == -1)
		return (perror_mnsh(127, 2, args[0], strerror(ENOENT)));
	if (S_ISDIR(st.st_mode))
		return (perror_mnsh(126, 2, args[0], strerror(EISDIR)));
	if (!(S_IXUSR & st.st_mode))
		return (perror_mnsh(126, 2, args[0], strerror(EACCES)));
	return (0);
}
