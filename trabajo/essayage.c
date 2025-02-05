#include "minishell.h"

int ft_strncmp_exact(char *s1, char *s2, size_t len)
{
    int i;

    i = 0;
    while (len > 0)
    {
        if (!s1[i])
            s1[i] = 0;
        if (!s2[i])
            s2[i] = 0;
        if (s1[i] == s2[i])
        {
            i++;
            len--;
            continue;
        }
        else
    }
    

}



int main()
{





    return (0);
}

