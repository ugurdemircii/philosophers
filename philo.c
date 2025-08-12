#include "philo.h"

unsigned long ft_get_time(void)
{
	struct  timeval tv;
	unsigned long time;

	gettimeofday(&tv,NULL);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

void safe_mutex_print(char *str,t_philo *philo)
{
    if (philo->data->dead_flag)
        return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%d %s\n",philo->philo_id,str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
void think_and_sleep(t_philo *philo)
{
	safe_mutex_print("is sleeping",philo);
	usleep(philo->data->time_to_sleep * 1000);
	safe_mutex_print("is thinking",philo);
}


void take_fork_and_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_mutex_print("has taken a fork",philo);
	pthread_mutex_lock(philo->right_fork);
	safe_mutex_print("has taken a fork",philo);
	pthread_mutex_lock(&philo->eat_mutex);
	philo->last_eat_time = ft_get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_mutex);
	safe_mutex_print("is eating",philo);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}



void life_of_philo(t_philo *philo)
{
	take_fork_and_eat(philo);
	think_and_sleep(philo);
}
void *is_anyone_dead(void *arg)
{
	t_data *simulation;
	int i;
	
	simulation = (t_data *)arg;
	while (1)
	{
		i = 0;
		while(i < simulation->number_of_philo)
		{
			if (ft_get_time() - simulation->philo[i].last_eat_time >= (unsigned long)simulation->time_to_die)
			{
				pthread_mutex_lock(&simulation->dead_mutex);
				simulation->dead_flag = 1;
				pthread_mutex_unlock(&simulation->dead_mutex);
				// safe_mutex_print("is dead",&simulation->philo[i]);
				printf("%d is dead\n",simulation->philo[i].philo_id);
				return NULL;
			}
			i++;
		}
	}
}

void *philo_loop(void *arg)
{
	t_philo *philo;
	t_data *simulation;
	pthread_t check_dead;

	philo = (t_philo *)arg;
	simulation = philo->data;
	while (!simulation->dead_flag)
	{
		life_of_philo(philo);
	}
	return NULL;

}

void start_thread(t_data *simulation)
{
	int i;
	int left_fork;
	int right_fork;
	pthread_t check_dead;

	i = -1;
	while (++i < simulation->number_of_philo)
	{
		pthread_create(&simulation->philo[i].thread,NULL,philo_loop,&simulation->philo[i]);
		usleep(1000);
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
		philo[i].last_eat_time = simulation->start_time;
		philo[i].eat_count = 0;
		philo[i].data = simulation;
		if (philo[i].philo_id % 2)
		{
			philo[i].left_fork = &simulation->forks[i];
			philo[i].right_fork = &simulation->forks[(i + 1) % simulation->number_of_philo];
		}
		else
		{
			philo[i].left_fork = &simulation->forks[(i + 1) % simulation->number_of_philo];
			philo[i].right_fork = &simulation->forks[i];
		}
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
	simulation->dead_flag = 0;
	simulation->forks = malloc(sizeof(pthread_mutex_t)* simulation->number_of_philo);
	pthread_mutex_init(simulation->forks,NULL);
	pthread_mutex_init(&simulation->dead_mutex,NULL);
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
	pthread_t check_dead;
	int i;


	i = 0;
	simulation = init_data(simulation,argv);
	printf("%lu\n",simulation->start_time);
	start_thread(simulation);
	pthread_create(&check_dead,NULL,is_anyone_dead,simulation);
	while (i < simulation->number_of_philo)
	{
		pthread_join(simulation->philo[i].thread,NULL);
		i++;
	}
	pthread_join(check_dead,NULL);
}