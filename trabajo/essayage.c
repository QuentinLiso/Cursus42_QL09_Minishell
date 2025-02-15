#include "minishell.h"


int main() {
    char *input;

	if (isatty(STDIN_FILENO))
    	input = readline("Enter something: ");
	else
    	input = ft_strdup("bonjour");  // No prompt in non-interactive mode
    if (!input) {  // Handle NULL case
        printf("EOF or error\n");
        return 1;
    }
    write(1, input, strlen(input));  // Make sure input is valid before writing
    free(input);  // Avoid memory leak

    return 0;
}