#include "minishell.h"

/* ====== Nouvelles fonctions :
char *get_env(char *__NAME)
Equivalent dans shell de la commande : echo $NAME
Liste des variables d'environnement avec la commande export

*/



t_cmdlst	*parsing(char *command)
{
	t_cmdlst	*command_list;
	t_list		*args;

	args = NULL;
	if (check_quotes(command))
		return (NULL);
	if (!split_args(&args, command)) // TODO
		return (NULL);
	
	return (command_list);
}

int	split_args(t_list **args, char *cmd)
{
	int		i;
	char	*arg;
	int		end_index;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == ' ' || cmd[i] == '\t')
			continue;
		if (is_special(cmd[i]))
			end_index = get_end_index(cmd + cur, -1); // TODO
	}
	return (1);
}

bool	is_special(char c)
{
	int	i;
	char	specials[8];
	specials[0] = '\'';
	specials[1] = '"'; 
	specials[2] = '<'; 
	specials[3] = '&'; 
	specials[4] = '|'; 
	specials[5] = '>'; 
	specials[6] = ';';
	specials[7] = '\0';
	i = -1;
	while (++i < 7)
		if (specials[i] == c)
			return (true);
	return (false);
}

int	get_end_index(char *str, int e_end) // TODO
{
	int		end_index;
	char	**splitchar;
	int		i;

	if (e_end == 0)
		return (0);
	splitchar = ft_split("&& || | << < >> > & ;", ' ');
	if (!splitchar)
		return (0);
	end_index = get_quotes_end(str, e_end);
	if (end_index == 0)
		return (strarr_free(splitchar));
	if (end_index != -2)
		return (end_index + strarr_free(splitchar));
	i = -1;
	while (splitchar[++i])
	{
		if (e_end > -1 && index_of(str, splitchar[i], 1) < e_end)
			return (index_of())
	}

}

int	get_quotes_end(char *str, int e_end) // OK
{
	int	end_index;

	end_index = -2;
	if (str[0] = '\'')
		end_index = get_arg_end(str, index_of(str + 1, "'", 1) + 2);
	else if (e_end > -1 && index_of(str, "'", 1) < e_end)
		end_index = get_arg_end(str, index_of(str + 1, "'", 2) + 2);
	else if (str[0] == '"')
		end_index = get_arg_end(str, index_of(str + 1, "\"", 1) + 2);
	else if (e_end > -1 && index_of(str, "\"", 1) < e_end)
		end_index = get_arg_end(str, index_of(str + 1, "\"", 2) + 2);
	return (end_index);
}

int	get_arg_end(char *str, int quote_index) // OK
{
	char	**splitchar;
	int		end_index;
	int		test_index;
	int		i;

	splitchar = ft_split("&& || | << < >> > & ;", ' ');
	if (!splitchar)
		return (0);
	end_index = index_of_newline(str + quote_index);
	i = -1;
	while (splitchar[++i])
	{
		test_index = index_of(str + quote_index, splitchar[i], 1);
		if (test_index < end_index)
		{
			strarr_free(splitchar);
			return (test_index + quote_index);
		}
	}
	strarr_free(splitchar);
	return (end_index + quote_index);
}

int	index_of_newline(char *str) // OK
{
	int	i;
	int	tmp;

	i = index_of(str, " ", 1);
	tmp = index_of(str, '\t', 1);
	if (tmp || str[0] == '\t')
		if (tmp < i)
			i = tmp;
	return (i);
}

int	index_of(char *str, char *search, int n) // OK
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ft_strncmp(str + i, search, ft_strlen(search)) == 0)
			n--;
		if (n <= 0)
			return (i);
	}
	return (i);
}

int	strarr_free(char **array) // OK
{
	int	i;

	i = -1;
	if (array)
	{
		while (array[++i])
			free(array[i]);
		free(array);
	}
	return (0);
}

int main(int ac, char **av, char **env)
{
	t_mem		*mem;
	t_cmdlst	*command_list;
	char 	*str;
	

	(void)ac;
	(void)av;
	mem = init_mem();
	mem->path_tab = ft_split(getenv("PATH"), ':');
	mem->my_env = copy_env(env);
	printf(MINISHELL);

	while (42)
	{
		str = take_input(mem);
		if (!str)
			continue;
		command_list = parsing(str);
		(void)command_list;
	}



	// char	*cwd;
	// while (1)
	// {
	// 	cwd = getcwd(NULL, 0);
	// 	printf(R "%s 🐚 : " RST, cwd);
	// 	buf = readline("$> ");	
	// 	printf("cmd : %s\n", buf);
	// 	if (strcmp(buf, "cd ..") == 0)
	// 		chdir("..");
	// }

	return (EXIT_SUCCESS);
}
