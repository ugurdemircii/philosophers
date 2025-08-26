/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:37:20 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/27 02:19:06 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	think_and_sleep(t_philo *philo)
{
	safe_mutex_print("is sleeping", philo);
	ft_usleep(philo->data->time_to_sleep, philo);
	safe_mutex_print("is thinking", philo);
}

static void	take_fork_and_eat(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	if (philo->left_forkk < philo->right_forkk)
	{
		first_fork = philo->left_forkk;
		second_fork = philo->right_forkk;
	}
	else
	{
		first_fork = philo->right_forkk;
		second_fork = philo->left_forkk;
	}
	pthread_mutex_lock(&philo->data->forks[first_fork]);
	safe_mutex_print("has taken a fork", philo);
	pthread_mutex_lock(&philo->data->forks[second_fork]);
	safe_mutex_print("has taken a fork", philo);
	pthread_mutex_lock(&philo->eat_mutex);
	safe_mutex_print("is eating", philo);
	philo->last_eat_time = ft_get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_mutex);
	ft_usleep(philo->data->time_to_eat, philo);
	pthread_mutex_unlock(&philo->data->forks[second_fork]);
	pthread_mutex_unlock(&philo->data->forks[first_fork]);
}

void	*philo_loop(void *arg)
{
	t_philo		*philo;
	t_data		*simulation;

	philo = (t_philo *)arg;
	simulation = philo->data;
	while (1)
	{
		take_fork_and_eat(philo);
		if (is_simulation_end(philo))
			break ;
		think_and_sleep(philo);
		
	}
	return (NULL);
}

static void	free_and_destroy(t_data *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->number_of_philo)
	{
		pthread_mutex_destroy(&simulation->philo[i].eat_mutex);
		pthread_mutex_destroy(&simulation->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&simulation->dead_mutex);
	pthread_mutex_destroy(&simulation->print_mutex);
	free(simulation->philo);
	free(simulation->forks);
	free(simulation);
}

int	main(int argc, char **argv)
{
	t_data		*simulation;
	pthread_t	check_dead;
	int			i;

	i = 0;
	if ((argc != 5 && argc != 6) || input_parse(argv) == -1)
		return(1);
	simulation = init_data(argv);
	start_thread(simulation);
	pthread_create(&check_dead, NULL, is_anyone_dead, simulation);
	while (i < simulation->number_of_philo)
	{
		pthread_join(simulation->philo[i].thread, NULL);
		i++;
	}
	pthread_join(check_dead, NULL);
	free_and_destroy(simulation);
	return (1);
}
