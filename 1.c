#include "minishell.h"

// ========================================================

t_mem	*init_mem(void)
{
	t_mem	*mem;

	mem = malloc(sizeof(t_mem));
	if (!mem)
		return (NULL);
	mem->exec_loop = malloc(sizeof(t_exec_loop));
	if (!mem->exec_loop)
		return (NULL);
	mem->exec_loop->redirect_file_path = NULL;
	mem->exec_loop->right_path = NULL;
	mem->my_env = NULL;
	mem->path_tab = NULL;
	mem->tmpfile = "buzz";
	mem->exit_statue = 0;
	mem->last_cmd_exit_statue = 0;
	return (mem);
}

char	**copy_env(char **env)
{
	char	**my_env;
	int		i;
	int		len;

	len = 0;
	i = -1;
	while (env[++i])
		len++;
	my_env = ft_calloc(len + 2, sizeof(char *));
	if (!my_env)
		return (NULL);
	i = -1;
	while (env[++i])
		my_env[i] = ft_strdup(env[i]);
	my_env[i] = NULL;
	return (my_env);
}


// ========================================================

char	*take_input(t_mem *mem)
{
	char	*buf;
	char	*prompt;

	prompt = get_prompt(mem);
	buf = readline(prompt);
	free(prompt);
	if (!buf)
	{
		printf("\n");
		exit(1);
	}
	if (ft_strlen(buf) == 0)
	{
		free(buf);
		return (NULL);
	}
	add_history(buf);
	return (buf);
}

char	*get_prompt(t_mem *mem)
{
	char	*prompt;
	char	*pwd;
	char	*temp;
	char	*start;

	pwd = my_getenv(mem->my_env, "PWD");
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

char	*my_getenv(char **env, char *elem)
{
	char	*str;
	int		i;
	int		elem_len;

	elem_len = ft_strlen(elem);
	i = -1;
	while (env[++i])
		if (!strcmp_len(elem, env[i]))
			break;
	if (!env[i])
		return (NULL);
	str = ft_strdup(env[i] + elem_len + 1);
	if (!str)
		return (NULL);
	return (str);
}

int	strcmp_len(char *s1, char *s2)
{
	return (ft_strncmp(s1, s2, ft_strlen(s1)));
}