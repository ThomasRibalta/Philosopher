#include "philosopher.h"

void free_all(t_info *info)
{
    free(info->philos);
    sem_close(info->forks);
    sem_close(info->writing);
    sem_close(info->eat);
    sem_unlink("/forks");
	sem_unlink("/write");
    sem_unlink("/eat");
    free(info);
}

int philo_is_death(t_philo *philo)
{
    long long millis;

    millis = get_time() - ((philo->last_time_eat.tv_sec * 1000) + (philo->last_time_eat.tv_usec / 1000));
    if (millis >= philo->info->time_die)
    {
        return 1;
    }
    return 0;
}

void kill_all(t_info *info)
{
    int i;

    i = -1;
    while (++i != info->n_philo)
    {
        if (info->philos[i].phil_fork_id != 0)
            kill(info->philos[i].phil_fork_id, SIGKILL);
    }
}

void *mind(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    while (1)
    {
        sem_wait(philo->info->eat);
        if (philo_is_death(philo) || philo->n_eat == philo->info->eat_interval)
        {
            set_state(philo, 4);
            philo->info->end_process = 1;
            exit(1);
        }
        sem_post(philo->info->eat);
        if (philo->info->end_process)
            break;
    }
    return (NULL);
}
