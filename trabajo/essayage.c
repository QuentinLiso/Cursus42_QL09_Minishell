#include "minishell.h"


int	main()
{
    char *file = "zzzz";
    int   fd_wr;
    int   fd_rd;
    char    *line;

    fd_wr = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fd_rd = open(file, O_RDONLY);
    unlink(file);
    ft_putstr_fd("bonjour", fd_wr);
    close(fd_wr);
    line = get_next_line(fd_rd);
    close(fd_rd);
    printf("Line : %s\n", line);
	return (0);
}