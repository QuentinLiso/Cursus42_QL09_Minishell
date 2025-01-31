#include <stdio.h>

int main()
{
	char str[] = "\' \"' \'\'' ' \'";
	int	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			printf("1 : %c\n", str[i]);
		else
			printf("0 : %c\n", str[i]);
		i++;
	}
	return (0);
}