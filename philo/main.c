/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomas.rba <thomas.rba@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:29:23 by toto              #+#    #+#             */
/*   Updated: 2024/06/20 22:08:05 by thomas.rba       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	check_args(char **args)
{
	while (*args)
	{
		if (atoi(*args) <= 0)
			return (0);
		args++;
	}
	return (1);
}

static void	make_philo(t_global **global, int i)
{
	pthread_mutex_init((*global)->philo_mutex + i, NULL);
	(*global)->philos[i].info = (*global)->info;
	(*global)->philos[i].mutex = (*global)->philo_mutex + i;
	(*global)->philos[i].id = i;
	(*global)->philos[i].global = (*global)->mutex;
	gettimeofday(&(*global)->philos[i].last_time_eat, NULL);
	(*global)->philos[i].is_dead = false;
	(*global)->philos[i].n_eat = 0;
	pthread_mutex_init((*global)->fork + i, NULL);
	(*global)->philos[i].fork = (*global)->fork;
	pthread_create(&((*global)->philos_threads[i]), NULL, &philosopher_behavior,
		&(*global)->philos[i]);
}

static void	create_philosophers(t_global **g)
{
	int	i;

	i = -1;
	(*g)->philos = malloc(sizeof(t_philo) * (*g)->info.n_philo);
	memset((*g)->philos, 0, sizeof(t_philo) * (*g)->info.n_philo);
	(*g)->mutex = malloc(sizeof(pthread_mutex_t));
	memset((*g)->mutex, 0, sizeof(pthread_mutex_t));
	(*g)->philo_mutex = malloc(sizeof(pthread_mutex_t)
			* (*g)->info.n_philo);
	memset((*g)->philo_mutex, 0, sizeof(pthread_mutex_t)
		* (*g)->info.n_philo);
	(*g)->fork = malloc(sizeof(pthread_mutex_t) * (*g)->info.n_philo);
	memset((*g)->fork, 0, sizeof(pthread_mutex_t) * (*g)->info.n_philo);
	(*g)->philos_threads = malloc(sizeof(pthread_t)
			* ((*g)->info.n_philo + 1));
	memset((*g)->philos_threads, 0, sizeof(pthread_t)
		* ((*g)->info.n_philo + 1));
	pthread_mutex_init((*g)->mutex, NULL);
	while (++i != (*g)->info.n_philo)
		make_philo(g, i);
	pthread_create(&((*g)->philos_threads[i]), NULL, &mind, g);
	i = -1;
	while (++i != (*g)->info.n_philo)
		pthread_join((*g)->philos_threads[i], NULL);
	pthread_join((*g)->philos_threads[i], NULL);
}

static void	free_all(t_global **global)
{
	int	i;

	i = -1;
	usleep(1000);
	while (++i != (*global)->info.n_philo)
	{
		pthread_mutex_destroy((*global)->philo_mutex + i);
		pthread_mutex_destroy((*global)->fork + i);
	}
	pthread_mutex_destroy((*global)->mutex);
	free((*global)->philos);
	free((*global)->mutex);
	free((*global)->fork);
	free((*global)->philo_mutex);
	free((*global)->philos_threads);
	free(*global);
}

int	main(int ac, char **av)
{
	t_global	*global;

	if ((ac < 5 || ac > 6) && !check_args(av))
		return (aff_error(3));
	else
	{
		global = malloc(sizeof(t_global));
		if (!global)
		{
			printf("Error: Memory allocation failed\n");
			return (1);
		}
		if (aff_error(init_global_info(&global, ac, av)) == 0)
		{
			create_philosophers(&global);
			free_all(&global);
		}
		else
		{
			free(global);
			return (1);
		}
	}
	return (0);
}
