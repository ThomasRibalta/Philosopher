/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:39:56 by toto              #+#    #+#             */
/*   Updated: 2024/06/20 14:32:41 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	set_state(t_philo *philo, int state)
{
	sem_wait(philo->info->writing);
	put_long_long(get_time() - philo->info->start_time);
	write(1, "ms philosophe n^", 3);
	put_long_long((long long)philo->id);
	if (state == 0)
		write(1, " is thinking\n", 13);
	else if (state == 1)
		write(1, " is eating\n", 11);
	else if (state == 2)
		write(1, " is sleeping\n", 13);
	else if (state == 3)
		write(1, " has taken a fork\n", 18);
	else if (state == 4)
		write(1, " died\n", 6);
	sem_post(philo->info->writing);
}

static void	take_forks(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	sem_wait(info->forks);
	sem_wait(info->forks);
	set_state(philo, 3);
}

static void	put_forks(t_philo *philo)
{
	sem_post(philo->info->forks);
	sem_post(philo->info->forks);
}

void	philo_eat(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	sem_wait(info->eat);
	set_state(philo, 1);
	philo->last_time_eat = get_time();
	sem_post(info->eat);
	ft_usleep(info->time_eat);
	philo->n_eat++;
	put_forks(philo);
}

void	philosopher_behavior(void *philo)
{
	pthread_t	checker;
	t_philo		*philo2;
	t_info		*info;

	philo2 = philo;
	info = philo2->info;
	philo2->last_time_eat = get_time();
	pthread_create(&checker, NULL, mind, philo);
	if (philo2->id % 2)
		usleep(15000);
	while (!(info->end_process))
	{
		take_forks(philo2);
		philo_eat(philo2);
		if (philo2->n_eat >= info->eat_interval && info->eat_interval != -1)
			break ;
		set_state(philo2, 2);
		ft_usleep(info->time_sleep);
		set_state(philo2, 0);
	}
	pthread_join(checker, NULL);
	if (info->end_process)
		exit(1);
	exit(0);
}
