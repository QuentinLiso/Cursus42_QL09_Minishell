#include "minishell.h"


int main(int ac, char **av, char **env)
{    
	char	*s = "bonjour";
	char	*start = s;

	s+=3;
	
	char	*substr = ft_substr(start, 0, s - start);
	printf("%s\n", substr);
	(void)ac; (void)av; (void)env;

    return 0;
}

