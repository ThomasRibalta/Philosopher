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

static int	init_global_info(t_info *info, int ac, char **av)
{
	info->n_philo = ft_atoi(av[1]);
	if (info->n_philo < 1 || info->n_philo > 200)
		return (1);
	info->time_die = ft_atoi(av[2]);
	info->time_eat = ft_atoi(av[3]);
	info->time_sleep = ft_atoi(av[4]);
	if (info->time_die <= 60 || info->time_eat <= 60 || info->time_sleep <= 60)
		return (2);
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
	return (0);
}

static void	init_philos(t_info *info)
{
	int	i;

	i = -1;
	while (++i != info->n_philo)
	{
		info->philos[i].id = i;
		info->philos[i].n_eat = 0;
		info->philos[i].info = info;
		info->philos[i].last_time_eat = 0;
	}
}

static void	launch_processes(t_info *info)
{
	t_philo	*philo;
	int		i;
	int		state;

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
			kill_all(info);
		if (state != 0)
			break ;
	}
}

int	main(int ac, char **av)
{
	t_info	info;

	if ((ac < 5 || ac > 6) && !check_args(av))
	{
		return (aff_error(3));
	}
	else
	{
		if (aff_error(init_global_info(&info, ac, av)) != 0)
			return (0);
		init_philos(&info);
		launch_processes(&info);
		free_all(&info);
	}
	return (0);
}
