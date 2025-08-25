#include "philo.h"


void safe_mutex_print(char *str,t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead_flag == 0)
		printf("%lld %d %s\n",ft_get_time() - philo->data->start_time ,philo->philo_id,str);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

long long ft_get_time(void)
{
	struct  timeval tv;
	long long time;

	gettimeofday(&tv,NULL);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

void ft_usleep(int time_ms,t_philo *philo)
{
	long start_time;

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
	return;
}

void start_thread(t_data *simulation)
{
	int i;

	i = -1;
	while (++i < simulation->number_of_philo)
	{
		pthread_create(&simulation->philo[i].thread,NULL,philo_loop,&simulation->philo[i]);
		usleep(1);
	}
}