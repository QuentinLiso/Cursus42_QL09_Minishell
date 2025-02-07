#include "minishell.h"

bool	ft_str_contains(char *s, char c)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (true);
	return (false);
}

int main()
{
    char *str;
    bool b;

    str = "bonjour les amis";
    b = ft_str_contains(str, 'f');
    if (b)
        printf("true\n");
    else
        printf("false\n");
    return (0);
}

