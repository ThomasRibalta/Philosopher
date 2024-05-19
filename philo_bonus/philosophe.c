#include "philosopher.h"

void set_state(t_philo *philo, int state)
{
    long long millis;

    millis = get_time() - ((philo->info->start_time.tv_sec * 1000) + (philo->info->start_time.tv_usec / 1000));
    sem_wait(philo->info->writing);
    put_long_long(millis);
    write(1, "ms philosophe n^", 3);
    put_long_long((long long) philo->id);
    if (state == 0)
        write(1, " is thinking\n", 13);
    else if (state == 1)
        write(1, " is eating\n", 11);
    else if (state == 2)
        write(1, " is sleeping\n", 13);
    else if (state == 3)
        write(1, " has taken a fork\n", 18);
    else if (state == 4)
    {
        write(1, " died\n", 6);
        return;
    }
    sem_post(philo->info->writing);
}

static void take_forks(t_philo *philo)
{
    sem_wait(philo->info->forks);
    sem_wait(philo->info->forks);
    set_state(philo, 3);
}

static void put_forks(t_philo *philo)
{
    sem_post(philo->info->forks);
    sem_post(philo->info->forks);
}

void philo_eat(t_philo *philo)
{
    sem_wait(philo->info->eat);
    set_state(philo, 1);
    gettimeofday(&(philo->last_time_eat), NULL);
    sem_post(philo->info->eat);
    ft_usleep(philo->info->time_eat);
    philo->n_eat++;
    put_forks(philo);
    set_state(philo, 2);
    ft_usleep(philo->info->time_sleep);
    set_state(philo, 0);
}

void philosopher_behavior(t_philo *philo)
{
    pthread_t philo_thread;

    pthread_create(&philo_thread, NULL, &mind, philo);
    while (!philo->info->end_process)
    {
        take_forks(philo);
        philo_eat(philo);
    }
    pthread_join(philo_thread, NULL);
    if (philo->info->end_process)
        exit(1);
    exit(0);
}