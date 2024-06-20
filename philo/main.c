/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toto <toto@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:29:23 by toto              #+#    #+#             */
/*   Updated: 2024/05/19 17:33:41 by toto             ###   ########.fr       */
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

static void	create_philosophers(t_global **global) {
    int i = -1;

    (*global)->philos = malloc(sizeof(t_philo) * (*global)->info.n_philo);
    if (!(*global)->philos)
        return;
    memset((*global)->philos, 0, sizeof(t_philo) * (*global)->info.n_philo);

    (*global)->mutex = malloc(sizeof(pthread_mutex_t));
    if (!(*global)->mutex) {
        free((*global)->philos);
        return;
    }
    memset((*global)->mutex, 0, sizeof(pthread_mutex_t));

    (*global)->philo_mutex = malloc(sizeof(pthread_mutex_t) * (*global)->info.n_philo);
    if (!(*global)->philo_mutex) {
        free((*global)->philos);
        free((*global)->mutex);
        return;
    }
    memset((*global)->philo_mutex, 0, sizeof(pthread_mutex_t) * (*global)->info.n_philo);

    (*global)->fork = malloc(sizeof(pthread_mutex_t) * (*global)->info.n_philo);
    if (!(*global)->fork) {
        free((*global)->philos);
        free((*global)->mutex);
        free((*global)->philo_mutex);
        return;
    }
    memset((*global)->fork, 0, sizeof(pthread_mutex_t) * (*global)->info.n_philo);

    (*global)->philos_threads = malloc(sizeof(pthread_t) * ((*global)->info.n_philo + 1));
    if (!(*global)->philos_threads) {
        free((*global)->philos);
        free((*global)->mutex);
        free((*global)->philo_mutex);
        free((*global)->fork);
        return;
    }
    memset((*global)->philos_threads, 0, sizeof(pthread_t) * ((*global)->info.n_philo + 1));

    pthread_mutex_init((*global)->mutex, NULL);

    while (++i != (*global)->info.n_philo) 
        make_philo(global, i);

    pthread_create(&((*global)->philos_threads[i]), NULL, &mind, global);

    i = -1;
    while (++i != (*global)->info.n_philo) 
        pthread_join((*global)->philos_threads[i], NULL);

    pthread_join((*global)->philos_threads[i], NULL);
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
	{
		return (0);
	}
	else
	{
		global = malloc(sizeof(t_global));
		if (!global)
		{
			printf("Error: Memory allocation failed\n");
			return (1);
		}
		init_global_info(&global, ac, av);
		create_philosophers(&global);
		free_all(&global);
	}
	return (0);
}
