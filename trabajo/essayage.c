#include "minishell.h"


int	main()
{
    char *line;

    while ((line = get_next_line(STDIN_FILENO)))
    {
        printf("Executing: %s", line);  // Replace this with actual execution
        free(line);
    }
	return (0);
}