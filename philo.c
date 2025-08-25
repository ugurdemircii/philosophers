#include "philo.h"



void think_and_sleep(t_philo *philo)
{
	safe_mutex_print("is sleeping",philo);
	ft_usleep(philo->data->time_to_sleep,philo);
	safe_mutex_print("is thinking",philo);
}


void take_fork_and_eat(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_forkk]);
		safe_mutex_print("has taken a fork",philo);
		pthread_mutex_lock(&philo->data->forks[philo->right_forkk]);			
		safe_mutex_print("has taken a fork",philo);
	}
	else
	{
		usleep(500);
		pthread_mutex_lock(&philo->data->forks[philo->right_forkk]);			
		safe_mutex_print("has taken a fork",philo);
		pthread_mutex_lock(&philo->data->forks[philo->left_forkk]);
		safe_mutex_print("has taken a fork",philo);
	}
	pthread_mutex_lock(&philo->eat_mutex);
	safe_mutex_print("is eating",philo);
	philo->last_eat_time = ft_get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_mutex);
	ft_usleep(philo->data->time_to_eat,philo);
    pthread_mutex_unlock(&philo->data->forks[philo->right_forkk]);
    pthread_mutex_unlock(&philo->data->forks[philo->left_forkk]);
}


void *philo_loop(void *arg)
{
	t_philo *philo;
	t_data *simulation;

	philo = (t_philo *)arg;
	simulation = philo->data;
	while (1)
	{
		take_fork_and_eat(philo);
		if (is_simulation_end(philo))
			break;
		think_and_sleep(philo);
	}
	return NULL;
}



void one_philo(t_data *simulation)
{
	printf("%lld 1 has taken a fork\n",ft_get_time() - simulation->start_time);
	usleep(simulation->time_to_die * 1000);
	printf("%lld 1 died\n",ft_get_time() - simulation->start_time);
	free(simulation);
	exit (0);
}


int main(int argc, char **argv)
{
	t_data *simulation;
	pthread_t check_dead;
	int i;

	i = 0;
	if (argc != 5 && argc != 6)
		exit(1);
	if (input_parse(argv) == -1)
	{
		printf("wrong input\n");
		exit(1);
	}
	simulation = init_data(argv);
	start_thread(simulation);
	pthread_create(&check_dead, NULL, is_anyone_dead, simulation);
	while (i < simulation->number_of_philo)
	{
		pthread_join(simulation->philo[i].thread,NULL);
		i++;
	}
	pthread_join(check_dead,NULL);
    for (i = 0; i < simulation->number_of_philo; i++) 
	{
        pthread_mutex_destroy(&simulation->philo[i].eat_mutex);
        pthread_mutex_destroy(&simulation->forks[i]);
    }
    pthread_mutex_destroy(&simulation->dead_mutex);
    pthread_mutex_destroy(&simulation->print_mutex);
    free(simulation->philo);
    free(simulation->forks);
    free(simulation);
}
