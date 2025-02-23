#include "minishell.h"

t_var	*alloc_env_var(char *key_eq_val)
{
	int		i;
	t_var	*var;

	var = ft_calloc(1, sizeof(t_var));
	if (!var)
		return (NULL);
	i = 0;
	while (key_eq_val[i] && key_eq_val[i] != '=')
		i++;
	var->key = ft_substr(key_eq_val, 0, i);
	if (!var->key)
		return (NULL);
	var->value = ft_strdup(&key_eq_val[i + 1]);
	if (!var->value)
		return (NULL);
	return (var);
}

int		free_env_var_ret(t_var *var, int ret)
{
	if (var)
	{
		ft_free_str(&var->key);
		ft_free_str(&var->value);
		free(var);
	}
	return (ret);
}

void	free_env_var(void *ptr)
{
	t_var	*var;

	var = (t_var *)ptr;
	if (var)
	{
		ft_free_str(&var->key);
		ft_free_str(&var->value);
		free(var);
	}
}

char	*get_env_var(t_list *env_lst, char *key)
{
	t_var	*var;
	
	if (!env_lst)
		return (NULL);
	while (env_lst)
	{
		var = (t_var *)env_lst->content;
		if (ft_strcmp(var->key, key) == 0)
			return (var->value);
		env_lst = env_lst->next;
	}
	return ("");
}

int		add_env_var(t_list **env_lst, char *key, char *value)
{
	t_var	*var;
	t_list	*new_elem;

	var = ft_calloc(1, sizeof(t_var));
	if (!var)
		return (ENOMEM);
	var->key = ft_strdup(key);
	if (var->key && value)
		var->value = ft_strdup(value);
	if (!var->key || (value && !var->value))
		return (free_env_var_ret(var, ENOMEM));
	new_elem = ft_lstnew(var);
	if (!new_elem)
		return (free_env_var_ret(var, ENOMEM));
	ft_lstadd_back(env_lst, new_elem);
	return (0);
}

int		edit_env_var(t_list **env_lst, char *key, char *value)
{
	t_var	*var;
	t_list	*iterator;

	iterator = *env_lst;
	while (iterator)
	{
		var = (t_var *)iterator->content;
		if (!ft_strcmp(var->key, key))
		{
			if (value)
			{
				ft_free_str(&var->value);
				var->value = ft_strdup(value);
				if (!var->value)
					return (ENOMEM);
			}
			return (0);
		}
		iterator = iterator->next;
	}
	return (add_env_var(env_lst, key, value));
}

void	display_env_var(void *ptr)
{
	t_var	*env_var;

	if (!ptr)
		return ;
	env_var = (t_var *)ptr;
	ft_putstr_fd(env_var->key, STDOUT_FILENO);
	ft_putchar_fd('=', STDOUT_FILENO);
	ft_putendl_fd(env_var->value, STDOUT_FILENO);
}

int		set_mnsh_paths(t_list *env_mnsh_list, char ***paths)
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
			(*paths)[i] = strjoin_free_s1((*paths)[i], "/");
			if (!(*paths)[i])
			{
				ft_free_strarray(paths);
				return (ENOMEM);
			}
		}
	}           
	return (0);
}
