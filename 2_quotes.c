#include "minishell.h"

int	check_quotes(char	*command)
{
	int	i;
	int	in_quotes;

	i = -1;
	in_quotes = 0;
	while (command[++i])
		set_in_quotes(command[i], &in_quotes); // TODO
	if (in_quotes != 0)
		print_error(1, NULL);
	return (in_quotes != 0);
}

int	set_in_quotes(char c, int *in_quotes)
{
	int	cquotes;

	cquotes = *in_quotes;
	if (c == '\'')
	{
		if (cquotes == 0)
			*in_quotes = 1;
		else if (cquotes == 1)
			*in_quotes = 0;
	}
	if (c == '"')
	{
		if (cquotes == 0)
			*in_quotes = 2;
		else if (cquotes == 2)
			*in_quotes = 0;
	}
	return (cquotes);
}

int	print_error(int error_code, char *content)
{
	if (error_code == 1)
		printf("\033[91m%s\033[0m\n",
			"minishell: parse error, quotes are never closed");
	else if (error_code == 2)
		printf("\033[91m%s '%s'\033[0m\n",
			"minishell: parse error, near", content);
	else if (error_code == 3)
		printf("\033[91m%s '%s'\033[0m\n",
			"minishell: the command cannot end with", content);
	return (1);

}