#include "minishell.h"

int main(int ac, char **av, char **env)
{
	(void)ac;(void)av;
	char *args[] = {"ls", "-l", NULL};
	execve("ls", args, env);
	return (0);
}
