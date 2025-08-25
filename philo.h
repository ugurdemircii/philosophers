/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:45:03 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/25 12:22:21 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_sphilo
{
	int				philo_id;
	long long		last_eat_time;
	int				eat_count;
	int				left_forkk;
	int				right_forkk;
	pthread_mutex_t	eat_mutex;
	pthread_t		thread;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				number_of_philo;
	int				must_eat;
	int				dead_flag;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
	t_philo			*philo;
}					t_data;

int					input_parse(char **argv);
void				*is_anyone_dead(void *arg);
void				*philo_loop(void *arg);
long long			ft_get_time(void);
void				safe_mutex_print(char *str, t_philo *philo);
void				ft_usleep(int time_ms, t_philo *philo);
t_data				*init_data(char **argv);
void				start_thread(t_data *simulation);
int					is_simulation_end(t_philo *philo);
int					ft_atoi(char *str);
int					ft_overflow(char *str);
void				*ft_calloc(size_t count, size_t size);

#endif