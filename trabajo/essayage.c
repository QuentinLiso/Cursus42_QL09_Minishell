#include "minishell.h"

void	set_quoted(char c, char *quote)
{
	if (*quote == 0 && (c == '\'' || c == '"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

int main()
{
	char	quote;
	char	*str = "bonj\"our' les amis";

	quote = 0;
	printf("%s\n", str);
	while (*str)
	{
		set_quoted(*str, &quote);
		if (!quote)
			write(1, "0", 1);
		else
			write(1, &quote, 1);
		str++;
	}
	printf("\n");
	

}
