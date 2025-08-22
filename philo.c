#include "philo.h"

unsigned long ft_get_time(void)
{
	struct  timeval tv;
	unsigned long time;

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
		usleep(100);
	}
	return;
}

void safe_mutex_print(char *str,t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->dead_mutex);
    if (philo->data->dead_flag)
	{
		pthread_mutex_unlock(&philo->data->dead_mutex);
		pthread_mutex_unlock(&philo->data->print_mutex);
        return ;
	}
	pthread_mutex_unlock(&philo->data->dead_mutex);
	printf("%lu %d %s\n",ft_get_time() - philo->data->start_time ,philo->philo_id,str);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
void think_and_sleep(t_philo *philo)
{
	safe_mutex_print("is sleeping",philo);
	ft_usleep(philo->data->time_to_sleep,philo);
	safe_mutex_print("is thinking",philo);
	usleep(500);
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
		pthread_mutex_lock(&philo->data->forks[philo->right_forkk]);			
		safe_mutex_print("has taken a fork",philo);
		pthread_mutex_lock(&philo->data->forks[philo->left_forkk]);
		safe_mutex_print("has taken a fork",philo);
	}
	pthread_mutex_lock(&philo->eat_mutex);
	safe_mutex_print("is eating",philo);
	philo->last_eat_time = ft_get_time();
	ft_usleep(philo->data->time_to_eat,philo);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_mutex);
    pthread_mutex_unlock(&philo->data->forks[philo->right_forkk]);
    pthread_mutex_unlock(&philo->data->forks[philo->left_forkk]);
}



void life_of_philo(t_philo *philo)
{
	take_fork_and_eat(philo);
	think_and_sleep(philo);
}

int check_must_eat_count(t_data *simulation)
{
	int i;
	int check_everyone;

	i = -1;
	check_everyone = 0;
	while(++i < simulation->number_of_philo)
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
void *is_anyone_dead(void *arg)
{
	t_data *simulation;
	int i;
	
	simulation = (t_data *)arg;
	while (1)
	{
		i = -1;
		if (simulation->must_eat != 0)
		{
			if (check_must_eat_count(simulation))
				return NULL;
		}
		while(++i < simulation->number_of_philo)
		{
			pthread_mutex_lock(&simulation->philo[i].eat_mutex);
			if (ft_get_time() - simulation->philo[i].last_eat_time >= (unsigned long)simulation->time_to_die)
			{
				pthread_mutex_unlock(&simulation->philo[i].eat_mutex);
				pthread_mutex_lock(&simulation->dead_mutex);
				simulation->dead_flag = 1;
				pthread_mutex_unlock(&simulation->dead_mutex);
				pthread_mutex_lock(&simulation->print_mutex);
				printf("%lu %d is dead\n",ft_get_time() - simulation->start_time,simulation->philo[i].philo_id);
				pthread_mutex_unlock(&simulation->print_mutex);
				return NULL;
			}
			pthread_mutex_unlock(&simulation->philo[i].eat_mutex);
		}
		usleep(500);
	}
}

void *philo_loop(void *arg)
{
	t_philo *philo;
	t_data *simulation;

	philo = (t_philo *)arg;
	simulation = philo->data;
	while (1)
	{
		pthread_mutex_lock(&simulation->dead_mutex);
		if (simulation->dead_flag == 1)
		{
			pthread_mutex_unlock(&simulation->dead_mutex);
			break;
		}
		pthread_mutex_unlock(&simulation->dead_mutex);
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
		if (i % 2 == 1)
			usleep(300);
		pthread_create(&simulation->philo[i].thread,NULL,philo_loop,&simulation->philo[i]);

	}
}

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
		// printf("left%d right%d\n",philo[i].left_forkk,philo[i].right_forkk);
		pthread_mutex_init(&philo[i].eat_mutex,NULL);
	}
	return ;
}

void one_philo(t_data *simulation)
{
	printf("%lu 1 has taken a fork\n",ft_get_time() - simulation->start_time);
	usleep(simulation->time_to_die * 1000);
	printf("%lu 1 is died\n",ft_get_time() - simulation->start_time);
	free(simulation);
	exit (0);
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
	if (simulation->number_of_philo == 1)
		one_philo(simulation);
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
	pthread_create(&check_dead, NULL, is_anyone_dead, simulation);
	start_thread(simulation);
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
