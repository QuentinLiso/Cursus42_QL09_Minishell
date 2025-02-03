#include "minishell.h"


int main()
{





    return (0);
}

char	*exec_ast_cmd_infiles(t_ast **node)
{
	t_infiles	*iterator;
	int			check_access;

	iterator = (*node)->infiles;
	if (!iterator->infile)
		return (NULL);
	while (iterator->infile)
	{
		printf("Iter infile : %s\n", iterator->infile);
		check_access = access(iterator->infile, O_RDONLY);
		if (check_access < 0)
		{
			perror("open failed");
			exit (errno);
		}
		if (iterator->next == NULL)
			return (iterator->infile);
		iterator = iterator->next;
	}
	return (iterator->infile);
}