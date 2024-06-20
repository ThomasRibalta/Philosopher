/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:36:52 by toto              #+#    #+#             */
/*   Updated: 2024/06/20 15:05:38 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long long	time_diff(long long time1, long long time2)
{
	return (time2 - time1);
}

void	free_all(t_info *info)
{
	sem_close(info->forks);
	sem_close(info->writing);
	sem_close(info->eat);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_eat");
}

void	kill_all(t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->n_philo)
	{
		kill(info->philos[i].phil_fork_id, 15);
	}
}

void	*mind(void *arg)
{
	t_philo	*philo;
	t_info	*info;

	philo = (t_philo *)arg;
	info = philo->info;
	while (1)
	{
		sem_wait(info->eat);
		if (time_diff(philo->last_time_eat, get_time()) > info->time_die)
		{
			set_state(philo, 4);
			info->end_process = 1;
			sem_wait(info->writing);
			exit(1);
		}
		sem_post(philo->info->eat);
		if (info->end_process)
			break ;
		usleep(1000);
		if (info->eat_interval != -1 && philo->n_eat >= info->eat_interval)
			break ;
	}
	return (NULL);
}

int	aff_error(int e)
{
	if (e == 1)
		printf("Le nombre de philosopheur est incorrect\n");
	else if (e == 2)
		printf("On vous a dit au dessus de 60ms pour tout les times\n");
	else if (e == 3)
		printf("./philo_bonus nbPhilo time time time (nb_eat)\n");
	return (e);
}
