/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:18:37 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/25 08:44:07 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo(t_data *simulation)
{
	int		i;
	t_philo	*philo;

	i = -1;
	simulation->philo = ft_calloc(1, sizeof(t_philo)
			* simulation->number_of_philo);
	philo = simulation->philo;
	while (++i < simulation->number_of_philo)
	{
		philo[i].philo_id = i + 1;
		philo[i].last_eat_time = simulation->start_time;
		philo[i].eat_count = 0;
		philo[i].data = simulation;
		philo[i].left_forkk = i;
		philo[i].right_forkk = (i + 1) % simulation->number_of_philo;
		pthread_mutex_init(&philo[i].eat_mutex, NULL);
	}
	return ;
}

static void	put_data(t_data *simulation, char **argv)
{
	simulation->start_time = ft_get_time();
	simulation->number_of_philo = ft_atoi(argv[1]);
	if (simulation->number_of_philo == 0)
	{
		free(simulation);
		exit(1);
	}
	simulation->time_to_die = ft_atoi(argv[2]);
	simulation->time_to_eat = ft_atoi(argv[3]);
	simulation->time_to_sleep = ft_atoi(argv[4]);
	simulation->dead_flag = 0;
	if (argv[5])
	{
		if (ft_atoi(argv[5]) != 0)
			simulation->must_eat = atoi(argv[5]);
		else
		{
			free(simulation);
			exit(1);
		}
	}
	else
		simulation->must_eat = 0;
}

t_data	*init_data(char **argv)
{
	int		i;
	t_data	*simulation;

	i = 0;
	simulation = ft_calloc(1, sizeof(t_data));
	put_data(simulation, argv);
	simulation->forks = ft_calloc(1, sizeof(pthread_mutex_t)
			* simulation->number_of_philo);
	while (i < simulation->number_of_philo)
	{
		pthread_mutex_init(&simulation->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&simulation->dead_mutex, NULL);
	pthread_mutex_init(&simulation->print_mutex, NULL);
	init_philo(simulation);
	return (simulation);
}
