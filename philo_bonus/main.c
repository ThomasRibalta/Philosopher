#include "philosopher.h"

static int check_args(char **args)
{
    int tmp;

    tmp = 0;
    while (*args)
    {
        if (atoi(*args) <= 0)
            return (0);
        args++;
    }
    return (1);
}

static void init_global_info(t_info *info, int ac, char **av){
    info->n_philo = ft_atoi(av[1]);
    info->time_die = ft_atoi(av[2]);
    info->time_eat = ft_atoi(av[3]);
    info->time_sleep = ft_atoi(av[4]);
    if (ac == 6)
        info->eat_interval = ft_atoi(av[5]);
    else
        info->eat_interval = -1;
    sem_unlink("/forks");
	sem_unlink("/write");
    sem_unlink("/eat");
	info->forks = sem_open("/forks", O_CREAT, S_IRWXU, info->n_philo);
	info->writing = sem_open("/write", O_CREAT, S_IRWXU, 1);
    info->eat = sem_open("/eat", O_CREAT, S_IRWXU, 1);
    info->philos = malloc(sizeof(t_philo) * info->n_philo);
    if (!info->philos)
    {
        printf("Error: Memory allocation failed\n");
        return;
    }
    gettimeofday(&info->start_time, NULL);
}

static void init_philos(t_info *info)
{
    int i;

    i = -1;
    while (++i != info->n_philo)
    {
        info->philos[i].id = i;
        info->philos[i].n_eat = 0;
        info->philos[i].state = 0;
        info->philos[i].phil_fork_id = i;
        info->philos[i].last_time_eat = info->start_time;
    }
}

static void launch_processes(t_info *info)
{
    int i;
    int state;

    i = -1;
    while (++i != info->n_philo)
    {
        info->philos[i].info = info;
        info->philos[i].phil_fork_id = fork();
        if (info->philos[i].phil_fork_id == 0)
            philosopher_behavior(&info->philos[i]);
    }
    i = -1;
    while (++i != info->n_philo)
    {
        waitpid(-1, &state, 0);
        if (state != 0)
        {
            kill_all(info);
            break;
        }
    }
}

// Main function
int main(int ac, char **av)
{
    t_info *info;

    if ((ac < 5 || ac > 6) && !check_args(av))
    {
        return 0;
    }
    else
    {
        info = malloc(sizeof(t_info));
        if (!info)
        {
            printf("Error: Memory allocation failed\n");
            return 1;
        }
        init_global_info(info, ac, av);
        init_philos(info);
        launch_processes(info);
        free_all(info);
    }
    return 0;
}