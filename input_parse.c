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

int has_int_overflow(char *str) 
{
    long result;
    int digit;
    
    result = 0;
    while ((*str == ' ') || (*str >= 9 && *str <= 13)) 
        str++;
    if (*str == '+') 
        str++;
    while (*str) 
    {
        digit = *str - '0';
        result = result * 10 + digit;
        if (result > INT_MAX) 
            return (-1);
        if ((*str == ' ') || (*str >= 9 && *str <= 13))
            break;
        str++;
    }
    return (0);
}


int	ft_atoi(char *str)
{
	int	count;
	int	sign;

	count = 0;
	sign = 1;
	while (*str == ' ' || *str == '\r' || *str == '\t' || *str == '\v')
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		count = count * 10 + (*str - 48);
		str++;
	}
	return (sign * count);
}

int space_check(char *str)
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
        if (space_check(argv[i + 1]) == -1)
            return (-1);
        if (has_int_overflow(argv[i + 1]) == -1)
            return (-1);
        i++;
    }
    return (0);
}
