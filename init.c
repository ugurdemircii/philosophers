#include "philo.h"



void init_philo(t_data *simulation)
{
	int i;
	t_philo *philo;
	
	i = -1;
	simulation->philo = calloc(1,sizeof(t_philo ) *simulation->number_of_philo);
	philo = simulation->philo;
	while (++i < simulation->number_of_philo)
	{
		philo[i].philo_id = i + 1;
		philo[i].last_eat_time = simulation->start_time;
		philo[i].eat_count = 0;
		philo[i].data = simulation;
		philo[i].left_forkk = i;
		philo[i].right_forkk = (i + 1) % simulation->number_of_philo;
		pthread_mutex_init(&philo[i].eat_mutex,NULL);
	}
	return ;
}

t_data *init_data(char **argv)
{
	int i;
	t_data *simulation;
	
	i = 0;
	simulation = calloc(1,sizeof(t_data));
	simulation->start_time = ft_get_time();
	simulation->number_of_philo = atoi(argv[1]);
	simulation->time_to_die = atoi(argv[2]);
	// if (simulation->number_of_philo == 1)
	// 	one_philo(simulation);
	simulation->time_to_eat = atoi(argv[3]);
	simulation->time_to_sleep = atoi(argv[4]);
	simulation->dead_flag = 0;
	simulation->forks = calloc(1, sizeof(pthread_mutex_t) * simulation->number_of_philo);
	while (i < simulation->number_of_philo)
	{
		pthread_mutex_init(&simulation->forks[i],NULL);
		i++;	
	}
	pthread_mutex_init(&simulation->dead_mutex,NULL);
	pthread_mutex_init(&simulation->print_mutex,NULL);
	if (argv[5])
	{
		// == 0 hata;
		simulation->must_eat = atoi(argv[5]);
	}
	else
		simulation->must_eat = 0;
	init_philo(simulation);
	return(simulation);
}
