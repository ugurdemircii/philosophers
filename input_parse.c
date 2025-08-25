/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:44:54 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/25 08:43:08 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	space_check(char *str)
{
	int		i;

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
	return (0);
}

static int	char_check(char *str)
{
	int	i;
	int	sign_count;

	i = 0;
	sign_count = 0;
	while (str[i])
	{
		if ((str[i] >= 9 && str[i] <= 13) || str[i] == 32 || (str[i] >= 48
				&& str[i] <= 57) || str[i] == 43)
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

int	input_parse(char **argv)
{
	int		i;

	i = 0;
	while (argv[i + 1])
	{
		if (char_check(argv[i + 1]) == -1)
			return (-1);
		if (space_check(argv[i + 1]) == -1)
			return (-1);
		if (ft_overflow(argv[i + 1]) == -1)
			return (-1);
		i++;
	}
	return (0);
}
