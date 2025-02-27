#include "minishell.h"


int	main()
{
	t_var	*env_var;

	env_var = alloc_env_var("COUCOU=salut");
	ft_putstr_fd(env_var->key, STDOUT_FILENO);
	ft_putchar_fd('=', STDOUT_FILENO);
	ft_putendl_fd(env_var->value, STDOUT_FILENO);

	char *str;
	str = env_var_to_char(env_var);
	printf("STR : %s\n", str);
	return (0);
}