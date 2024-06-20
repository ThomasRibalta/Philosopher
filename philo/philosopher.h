/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:34:21 by toto              #+#    #+#             */
/*   Updated: 2024/06/20 22:06:23 by thomas.rba       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_info
{
	int				n_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_interval;
	struct timeval	start_time;

}					t_info;

typedef struct s_philo
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	*global;
	pthread_mutex_t	*mutex;
	struct timeval	last_time_eat;
	t_info			info;
	int				n_eat;
	int				id;
	int				state;
	bool			is_dead;
}					t_philo;

typedef struct s_global
{
	t_info			info;
	t_philo			*philos;
	pthread_mutex_t	*fork;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*philo_mutex;
	pthread_t		*philos_threads;

}					t_global;

int					aff_error(int e);
int					ft_atoi(const char *str);
void				*philosopher_behavior(void *arg);
void				*mind(void *arg);
void				set_state(t_philo *philo, int state);
int					init_global_info(t_global **global, int ac, char **av);
void				take_forks(t_philo *philo);
void				put_forks(t_philo *philo);
void				ft_usleep(long int time_in_ms);
long long			get_time(void);

#endif
