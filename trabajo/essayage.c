#include "minishell.h"

void	ess_free_str(char **ptr)
{
	if (*ptr == NULL)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void		ess_free_strarray(char ***arr)
{
	int		i;

	i = -1;
	if ((*arr) == NULL)
		return ;
	while ((*arr)[++i])
		ess_free_str(&(*arr)[i]);
	free(*arr);
}

int main()
{
    char    **str;

    str = malloc(3 * sizeof(char *));
    str[0] = ft_strdup("bonjour");
    str[1] = ft_strdup("les amis");
    str[2] = NULL;
    ess_free_strarray(&str);

    char    **arr;

    arr = malloc(2 * sizeof(char *));
    arr[0] = ft_strdup("salut");
    arr[1] = ft_strdup("les copains");
    arr[2] = NULL;
    str = arr;
    printf("%s\n", str[0]);
    ess_free_strarray(&str);
    return (0);
}

