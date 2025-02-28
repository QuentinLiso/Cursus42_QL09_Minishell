#include "minishell.h"


int main(int ac, __attribute__((unused)) char **av, char **env)
{
	t_mnsh	mnsh;

	if (mnsh_initialization(&mnsh, ac, env))
	{
		free_all_mnsh(&mnsh);
		return (mnsh.last_exit_status);
	}
	loop_mnsh(&mnsh);
	free_all_mnsh(&mnsh);
	return (mnsh.last_exit_status);
}