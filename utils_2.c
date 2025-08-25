/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 08:28:47 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/25 08:31:35 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_bzero(void *s, size_t n)
{
	char	*str;
	size_t	i;

	i = 0;
	str = s;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

int	ft_overflow(char *str)
{
	long	result;
	int		digit;

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
			break ;
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

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
