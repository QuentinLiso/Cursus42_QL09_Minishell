#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_TOKEN_LEN 100

const char	*operators[] = {"&&", "||", ">>", "<<", "|", "<", ">", "&", ";"};
const int	num_operators = sizeof(operators) / sizeof(char *);

int	is_operator(const char *str)
{
	int	i;

	i = -1;
	while (++i < num_operators)
		if (strncmp(str, operators[i], strlen(operators[i])) == 0)
			return (strlen(operators[i]));
	return (0);
}

char	**tokenize(const char *input)
{
	char	**tokens;
	int		token_count;
	char	*s;
	int		op_len;
	char	*start;

	tokens = malloc(MAX_TOKENS * sizeof(char *));
	token_count = 0;
	s = (char *)input;
	while (*s)
	{
		if(isspace(*s))
		{
			s++;
			continue;
		}
		op_len = is_operator(s);
		if (op_len > 0)
		{
			tokens[token_count] = strndup(s, op_len);
			token_count++;
			s += op_len;
			continue;
		}
		start = s;
		while (*s && !isspace(*s) && !is_operator(s))
			s++;
		tokens[token_count] = strndup(start, s - start);
		token_count++;
	}
	tokens[token_count] = NULL;
	return (tokens);
}

void free_tokens(char **tokens)
{
	int	i;

	i = -1;
	while (tokens[++i])
		free(tokens[i]);
	free(tokens);
}


int main()
{
	const char	*command;
	char		**tokens;
	int			i;

	command = ">>>>>>>>";
	tokens = tokenize(command);
	i = -1;
	while (tokens[++i] != NULL)
		printf("Token %d : %s\n", i, tokens[i]);
	i = -1;
	while (tokens[++i] != NULL)
		free(tokens[i]);
	free(tokens);
	return (0);
}