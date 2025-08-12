#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>


typedef struct s_sphilo
{
    int philo_id;
    unsigned long last_eat_time;
    int eat_count;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t  eat_mutex;
    struct s_data *data;
} t_philo;


typedef struct s_data
{
    int time_to_eat;
    int time_to_die;
    int time_to_sleep;
    int number_of_philo;
    int must_eat;  
    int dead_flag;
    unsigned long  start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t dead_mutex;
    t_philo *philo;
}   t_data;



#endif