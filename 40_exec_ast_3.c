/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_exec_ast_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nefadli <nefadli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:11:43 by nefadli           #+#    #+#             */
/*   Updated: 2025/03/01 16:12:17 by nefadli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		set_execfile(char **execfile, char **args, t_mnsh *mnsh)
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

int		set_execfile_paths(t_list *env_mnsh_list, char ***paths)
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

int		check_execfile(char *execfile, char **args)
{
	struct stat st;
	
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

int		get_cmd_path(char **execfile, char *cmd, char **paths)
{
	int		i;
	char 	*buf;
	struct stat	st;

	i = -1;
	while (paths[++i])
	{
		buf = ft_strjoin(paths[i], cmd);
		if (!buf)
			return (perror_mnsh(ENOMEM, 1, "err malloc get cmd path"));
		if (stat(buf, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			*execfile = buf;
			return (0);
		}
		else
			free_str(&buf);
	}
	return (ENOENT);
}

int		ft_execve(char **execfile, char **args, t_mnsh *mnsh)
{
	char	**env;

	env = env_lst_to_arr(mnsh->env_mnsh_lst);
	if (!env)
		return (perror_mnsh(12, 1, "err malloc env for exec"));
	execve(*execfile, args, env);
	free_strarray(&env);
	free_str(execfile);	
	if (errno == ENOENT)
		return (perror_mnsh(127, 2, args[0], strerror(ENOENT)));
	else if (errno == EACCES)
		return (perror_mnsh(126, 2, args[0], strerror(EACCES)));
	else
		return (perror_mnsh(1, 2, args[0], strerror(errno)));
}