/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:37:09 by toto              #+#    #+#             */
/*   Updated: 2024/06/20 14:59:10 by toto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int check_args(char **args)
{
	while (*args)
	{
		if (atoi(*args) <= 0)
			return (0);
		args++;
	}
	return (1);
}

static void init_global_info(t_info *info, int ac, char **av)
{
	info->n_philo = ft_atoi(av[1]);
	info->time_die = ft_atoi(av[2]);
	info->time_eat = ft_atoi(av[3]);
	info->time_sleep = ft_atoi(av[4]);
	info->end_process = 0;
	if (ac == 6)
		info->eat_interval = ft_atoi(av[5]);
	else
		info->eat_interval = -1;
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_eat");
	info->forks = sem_open("/philo_forks", O_CREAT, S_IRWXU, info->n_philo);
	info->writing = sem_open("/philo_write", O_CREAT, S_IRWXU, 1);
	info->eat = sem_open("/philo_eat", O_CREAT, S_IRWXU, 1);
}

static void init_philos(t_info *info)
{
	int i;

	i = -1;
	while (++i != info->n_philo)
	{
		info->philos[i].id = i;
		info->philos[i].n_eat = 0;
		info->philos[i].info = info;
		info->philos[i].last_time_eat = 0;
	}
}

static void launch_processes(t_info *info)
{
	int i;
	t_philo *philo;
	int state;

	i = -1;
	philo = info->philos;
	info->start_time = get_time();
	while (++i != info->n_philo)
	{
		info->philos[i].phil_fork_id = fork();
		if (info->philos[i].phil_fork_id < 0)
			exit(1);
		else if (info->philos[i].phil_fork_id == 0)
			philosopher_behavior(&philo[i]);
		usleep(100);
	}
	i = -1;
	while (++i < info->n_philo)
	{
		waitpid(-1, &state, 0);
		if (state != 0)
		{
			kill_all(info);
			break;
		}
	}
	sem_close(info->forks);
	sem_close(info->writing);
	sem_close(info->eat);
	sem_unlink("/philo_forks");
	sem_unlink("/philo_write");
	sem_unlink("/philo_eat");
}

// Main function
int main(int ac, char **av)
{
	t_info info;

	if ((ac < 5 || ac > 6) && !check_args(av))
	{
		return (0);
	}
	else
	{
		init_global_info(&info, ac, av);
		init_philos(&info);
		launch_processes(&info);
		free_all(&info);
	}
	return (0);
}
