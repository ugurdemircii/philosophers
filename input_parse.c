#include "philo.h"

int ft_strlen(char *str)
{
    int i;

    i = 0;
    if (str == NULL || *str == '\0')
        return (0);
    while (str[i])
        i++;
    return (i);
}

int space_clear(char *str)
{
    int i;
    char *new_str;

    i = 0;
    while (str[i])
    {
        while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
            i++;
        if (str[i] == 43)
            i++;
        while ((str[i] >= 48 && str[i] <= 57))
            i++;
        while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
            i++;
        if (str[i] != '\0')
            return (-1);
    }
    return(0);
}

int char_check(char *str)
{
    int i;
    int sign_count;

    i = 0;
    sign_count = 0;
    while (str[i])
    {
        if ((str[i] >= 9 && str[i] <= 13) || str[i] == 32 || (str[i] >= 48 && str[i] <= 57)  || str[i] == 43)
        {
            if (str[i] == 43)
                sign_count++;
            i++;
        }
        else
            return (-1);
    }
    if (sign_count > 1)
        return (-1);
    return (0);
}

int input_parse(char **argv)
{
    int i;
    char **new_argv;
    char *str;

    i = 0;
    while (argv[i + 1])
    {
        if (char_check(argv[i + 1]) == -1)
            return (-1);
        if (space_clear(argv[i + 1]) == -1)
            return (-1);
        i++;
    }
    return (0);
}
