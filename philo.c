#include "philo.h"

unsigned long ft_get_time(void)
{
	struct  timeval tv;
	unsigned long time;

	gettimeofday(&tv,NULL);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

// void take_fork_and_eat(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->data->print_mutex);
// 	pthread_mutex_lock(&philo->data->forks[philo->philo_id]);
// 	pthread_mutex_lock(&philo->data->forks[(philo->philo_id + 1 ) % philo->data->number_of_philo]);
// 	printf("%d has taken a fork\n",philo->philo_id);
// 	printf("%d has taken a fork\n",philo->philo_id);
// 	pthread_mutex_unlock(&philo->data->forks[philo->philo_id]);
// 	pthread_mutex_unlock(&philo->data->forks[(philo->philo_id + 1 ) % philo->data->number_of_philo]);
// 	printf("%d is eating\n",philo->philo_id);
// 	pthread_mutex_lock(&philo->eat_mutex);
// 	philo->eat_count++;
// 	pthread_mutex_unlock(&philo->eat_mutex);
// 	usleep(philo->data->time_to_eat * 100);
// 	printf("sdfasdfds\n");
// 	pthread_mutex_unlock(&philo->data->print_mutex);

// }



void life_of_philo(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		printf("%d has taken a fork\n", philo->philo_id);
		pthread_mutex_lock(philo->right_fork);
		printf("%d has taken a fork\n", philo->philo_id);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		printf("%d has taken a fork\n", philo->philo_id);
		pthread_mutex_lock(philo->left_fork);
		printf("%d has taken a fork\n", philo->philo_id);
	}
	take_fork_and_eat(philo);
}


void *philo_loop(void *arg)
{
	t_philo *philo;
	t_data *simulation;

	philo = (t_philo *)arg;
	simulation = philo->data;


	while (1)
	{
		life_of_philo(philo);
	}

}

void start_thread(t_data *simulation)
{
	int i;
	int left_fork;
	int right_fork;

	i = -1;
	while (++i < simulation->number_of_philo)
	{
		pthread_create(&simulation->philo[i].thread,NULL,philo_loop,&simulation->philo[i]);
		usleep(100);
	}
}

void init_philo(t_data *simulation)
{
	int i;
	t_philo *philo;

	i = -1;
	simulation->philo = malloc(sizeof(t_philo ) *simulation->number_of_philo);
	philo = simulation->philo;
	while (++i < simulation->number_of_philo)
	{
		philo[i].philo_id = i + 1;
		philo[i].last_eat_time = 0;
		philo[i].eat_count = 0;
		philo[i].data = simulation;
		philo[i].left_fork = malloc(sizeof(pthread_mutex_t));
		philo[i].right_fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo[i].left_fork,NULL);
		pthread_mutex_init(philo[i].right_fork,NULL);
		pthread_mutex_init(&philo[i].eat_mutex,NULL);
	}
	return ;
}

t_data *init_data(t_data *simulation,char **argv)
{
	simulation = malloc(sizeof(t_data));
	simulation->start_time = ft_get_time();
	simulation->number_of_philo = atoi(argv[1]);
	simulation->time_to_die = atoi(argv[2]);
	simulation->time_to_eat = atoi(argv[3]);
	simulation->time_to_sleep = atoi(argv[4]);
	simulation->forks = malloc(sizeof(pthread_mutex_t)* simulation->number_of_philo);
	pthread_mutex_init(simulation->forks,NULL);
	if (argv[5])
		simulation->must_eat = atoi(argv[5]);
	else
		simulation->must_eat = 0;
	init_philo(simulation);
	return(simulation);
}


int main(int argc, char **argv)
{
	t_data *simulation;
	int i;


	i = 0;
	simulation = init_data(simulation,argv);
	printf("%lu\n",simulation->start_time);
	start_thread(simulation);
	while (i < simulation->number_of_philo)
	{
		pthread_join(simulation->philo[i].thread,NULL);
		i++;
	}

}