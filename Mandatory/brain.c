#include "philosopher.h"

// Function to check if any philosopher has died
static bool one_philo_is_death(t_global **global)
{
    int i;
    struct timeval current_time;
    long long millis;

    i = -1;
    while (++i != (*global)->info.n_philo)
    {
        gettimeofday(&current_time, NULL);
        millis = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000))
                - (((*global)->philos[i].last_time_eat.tv_sec * 1000) + ((*global)->philos[i].last_time_eat.tv_usec / 1000));
        if (millis >= (*global)->info.time_die && (*global)->philos[i].state != 1)
        {
            pthread_mutex_lock((*global)->philos[i].global);
            (*global)->philos[i].is_dead = true;
            pthread_mutex_unlock((*global)->philos[i].global);
            set_state(&(*global)->philos[i], 4);
            return true;
        }
    }
    return false;
}

// Function to kill all philosophers
static void kill_all_philos(t_global **global) {
    int i;

    i = -1;
    while (++i != (*global)->info.n_philo)
    {
        (*global)->philos[i].is_dead = true;
    }
}

// Function to check if the eat interval has ended
static bool end_eat_interval(t_global **global)
{
    int i;
    bool result;

    i = -1;
    while (++i != (*global)->info.n_philo)
    {
        if ((*global)->philos[i].n_eat >= (*global)->info.eat_interval && (*global)->info.eat_interval != -1)
        {
            result = true;
        }
        else
        {
            result = false;
            break;
        }
    }
    return result;
}

// Function for the thread responsible for monitoring philosopher states
void *mind(void *arg)
{
    t_global **global;

    global = (t_global **)arg;
    while (1) {
        if (one_philo_is_death(global) || end_eat_interval(global)) {
            pthread_mutex_lock((*global)->mutex);
            kill_all_philos(global);
            pthread_mutex_unlock((*global)->mutex);
            return NULL;
        }
    }
    return NULL;
}