/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:29:55 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/25 08:45:45 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_mutex_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead_flag == 0)
		printf("%lld %d %s\n", ft_get_time() - philo->data->start_time,
			philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

long long	ft_get_time(void)
{
	struct timeval	tv;
	long long		time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

void	ft_usleep(int time_ms, t_philo *philo)
{
	long	start_time;

	start_time = ft_get_time();
	while (ft_get_time() - start_time < time_ms)
	{
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead_flag == 1)
		{
			pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_mutex);
		usleep(500);
	}
	return ;
}

static void	*one_philo(void *arg)
{
	t_philo	*philo;
	t_data	*simulation;

	philo = (t_philo *)arg;
	simulation = philo->data;
	pthread_mutex_lock(&simulation->print_mutex);
	pthread_mutex_lock(&simulation->forks[0]);
	printf("%lld 1 has taken a fork\n", ft_get_time() - simulation->start_time);
	ft_usleep(simulation->time_to_die, simulation->philo);
	pthread_mutex_unlock(&simulation->forks[0]);
	printf("%lld 1 is died\n", ft_get_time() - simulation->start_time);
	pthread_mutex_unlock(&simulation->print_mutex);
	return (NULL);
}

void	start_thread(t_data *simulation)
{
	int	i;

	i = -1;
	if (simulation->number_of_philo == 1)
	{
		pthread_create(&simulation->philo[0].thread, NULL,
			one_philo, &simulation->philo[0]);
		return ;
	}
	while (++i < simulation->number_of_philo)
	{
		pthread_create(&simulation->philo[i].thread, NULL,
			philo_loop, &simulation->philo[i]);
		usleep(1);
	}
}
