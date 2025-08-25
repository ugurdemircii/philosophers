/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:24:37 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/25 09:40:05 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_simulation_end(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_mutex);
	if (philo->data->must_eat != 0)
	{
		if (philo->data->must_eat == philo->eat_count)
		{
			pthread_mutex_unlock(&philo->eat_mutex);
			return (1);
		}
	}
	pthread_mutex_unlock(&philo->eat_mutex);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead_flag == 1)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}

static int	check_must_eat_count(t_data *simulation)
{
	int	i;
	int	check_everyone;

	i = -1;
	check_everyone = 0;
	while (++i < simulation->number_of_philo)
	{
		pthread_mutex_lock(&simulation->philo[i].eat_mutex);
		if (simulation->philo[i].eat_count >= simulation->must_eat)
			check_everyone++;
		pthread_mutex_unlock(&simulation->philo[i].eat_mutex);
	}
	if (check_everyone == simulation->number_of_philo)
	{
		pthread_mutex_lock(&simulation->dead_mutex);
		simulation->dead_flag = 1;
		pthread_mutex_unlock(&simulation->dead_mutex);
		return (1);
	}
	return (0);
}

static void	announce_died(t_data *simulation, int i)
{
	pthread_mutex_lock(&simulation->dead_mutex);
	simulation->dead_flag = 1;
	pthread_mutex_unlock(&simulation->dead_mutex);
	pthread_mutex_lock(&simulation->print_mutex);
	if (simulation->number_of_philo != 1)
		printf("%lld %d died\n", ft_get_time() - simulation->start_time,
			simulation->philo[i].philo_id);
	pthread_mutex_unlock(&simulation->print_mutex);
}

void	*is_anyone_dead(void *arg)
{
	t_data	*simulation;
	int		i;

	simulation = (t_data *)arg;
	while (1)
	{
		i = -1;
		if (simulation->must_eat != 0 && check_must_eat_count(simulation) == 1)
			return (NULL);
		while (++i < simulation->number_of_philo)
		{
			pthread_mutex_lock(&simulation->philo[i].eat_mutex);
			if (ft_get_time() - simulation->philo[i].last_eat_time
				>= simulation->time_to_die)
			{
				pthread_mutex_unlock(&simulation->philo[i].eat_mutex);
				announce_died(simulation, i);
				return (NULL);
			}
			pthread_mutex_unlock(&simulation->philo[i].eat_mutex);
		}
		usleep(1000);
	}
}
