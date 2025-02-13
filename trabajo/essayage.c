#include "minishell.h"

int main()
{
	char	*str;

	str = readline("coucou > ");
	free(str);
	rl_clear_history();
	return (0);
}
