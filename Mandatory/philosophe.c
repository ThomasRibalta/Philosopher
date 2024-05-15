#include "philosopher.h"

// Function for philosopher to think
static void philo_think(t_philo *philo)
{
    if (philo->is_dead == true)
        return;
    set_state(philo, 0);
    return;
}

// Function for philosopher to sleep
static void philo_sleep(t_philo *philo)
{
    if (philo->is_dead == true)
        return;
    set_state(philo, 2);
    ft_usleep(philo->info.time_sleep);
    philo_think(philo);
}

// Function for philosopher to eat
static void philo_eat(t_philo *philo)
{
    set_state(philo, 1);
    pthread_mutex_lock(philo->mutex);
    gettimeofday(&(philo->last_time_eat), NULL);
    pthread_mutex_unlock(philo->mutex);
    if (philo->is_dead == true)
        return;
    ft_usleep(philo->info.time_eat);
    pthread_mutex_lock(philo->mutex);
    //gettimeofday(&(philo->last_time_eat), NULL);
    philo->n_eat++;
    pthread_mutex_unlock(philo->mutex);
    put_forks(philo);
    philo_sleep(philo);
}

// Function to simulate philosopher's behavior
void *philosopher_behavior(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    while (philo->is_dead == false)
    {
        if (philo->info.n_philo > 1)
        {
            take_forks(philo);
            philo_eat(philo);
        }
    }
    return NULL;
}