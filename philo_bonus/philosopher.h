/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:36:22 by toto              #+#    #+#             */
/*   Updated: 2024/06/20 13:19:16 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>

struct	s_info;

typedef struct s_philo
{
	struct s_info		*info;
	long long		last_time_eat;
	pid_t		phil_fork_id;
	int			n_eat;
	int			id;
}					t_philo;

typedef struct s_info
{
	int				n_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_interval;
	int				end_process;
	sem_t			*forks;
	sem_t			*writing;
	sem_t			*eat;
	t_philo			philos[300];
	long long		start_time;
}					t_info;

int					aff_error(int e);
int					ft_atoi(const char *str);
void				ft_usleep(long int time_in_ms);
void				*mind(void *arg);
void				kill_all(t_info *info);
void				free_all(t_info *info);
void				set_state(t_philo *philo, int state);
void				philosopher_behavior(void *philo);
void				put_long_long(long long n);
long long			get_time(void);
long long			time_diff(long long time1, long long time2);

#endif
