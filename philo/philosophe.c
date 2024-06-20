/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:31:58 by toto              #+#    #+#             */
/*   Updated: 2024/05/19 17:33:11 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	take_forks(t_philo *philo)
{
	if (philo->is_dead == true || philo->n_eat == philo->info.eat_interval)
		return ;
	pthread_mutex_lock(philo->fork + philo->id);
	pthread_mutex_lock(philo->mutex);
	if (philo->is_dead == true  || philo->n_eat == philo->info.eat_interval)
	{
		pthread_mutex_unlock(philo->mutex);
		pthread_mutex_unlock(philo->fork + philo->id);
		return ;
	}
	pthread_mutex_unlock(philo->mutex);
	pthread_mutex_lock(philo->fork + ((philo->id + 1)
			* (philo->id != philo->info.n_philo - 1)));
	pthread_mutex_lock(philo->mutex);
	if (philo->is_dead == true  || philo->n_eat == philo->info.eat_interval)
	{
		pthread_mutex_unlock(philo->mutex);
		pthread_mutex_unlock(philo->fork + philo->id);
		pthread_mutex_unlock(philo->fork + ((philo->id + 1)
				* (philo->id != philo->info.n_philo - 1)));
		return ;
	}
	pthread_mutex_unlock(philo->mutex);
	set_state(philo, 3);
}

static void	philo_think(t_philo *philo)
{
	if (philo->is_dead == true || philo->n_eat == philo->info.eat_interval)
		return ;
	set_state(philo, 0);
	return ;
}

static void	philo_sleep(t_philo *philo)
{
	if (philo->is_dead == true || philo->n_eat == philo->info.eat_interval)
		return ;
	set_state(philo, 2);
	ft_usleep(philo->info.time_sleep);
	philo_think(philo);
}

static void	philo_eat(t_philo *philo)
{
	if (philo->is_dead == true || philo->n_eat == philo->info.eat_interval)
		return ;
	set_state(philo, 1);
	pthread_mutex_lock(philo->mutex);
	gettimeofday(&(philo->last_time_eat), NULL);
	pthread_mutex_unlock(philo->mutex);
	if (philo->is_dead == true || philo->n_eat == philo->info.eat_interval)
		return ;
	ft_usleep(philo->info.time_eat);
	pthread_mutex_lock(philo->mutex);
	philo->n_eat++;
	pthread_mutex_unlock(philo->mutex);
	put_forks(philo);
	philo_sleep(philo);
}

void	*philosopher_behavior(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->is_dead == false)
	{
		if (philo->info.n_philo > 1)
		{
			take_forks(philo);
			philo_eat(philo);
		}
	}
	return (NULL);
}
