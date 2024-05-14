#include "philosopher.h"

// Function to check command line arguments
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

static void make_philo(t_global **global, int i)
{
        pthread_mutex_init((*global)->philo_mutex + i, NULL);
        (*global)->philos[i].info = (*global)->info;
        (*global)->philos[i].mutex = (*global)->philo_mutex + i;
        (*global)->philos[i].id = i;
        (*global)->philos[i].global = (*global)->mutex;
        (*global)->philos[i].last_time_eat = (*global)->info.start_time;
        (*global)->philos[i].is_dead = false;
        (*global)->philos[i].n_eat = 0;
        pthread_mutex_init((*global)->fork + i, NULL);
        (*global)->philos[i].fork = (*global)->fork + i;
        pthread_create(&((*global)->philos_threads[i]), NULL, &philosopher_behavior, &(*global)->philos[i]);
}

// Function to create philosophers
static void create_philosophers(t_global **global)
{
    int i;

    i = -1;
    (*global)->philos = malloc(sizeof(t_philo) * (*global)->info.n_philo);
    if (!(*global)->philos)
        return;
    (*global)->mutex = malloc(sizeof(pthread_mutex_t));
    if (!(*global)->mutex)
        return free((*global)->philos);
    (*global)->philo_mutex = malloc(sizeof(pthread_mutex_t) * (*global)->info.n_philo);
    if (!(*global)->philo_mutex)
        return free((*global)->philos), free((*global)->mutex);
    (*global)->fork = malloc(sizeof(pthread_mutex_t) * (*global)->info.n_philo);
    (*global)->philos_threads = malloc(sizeof(pthread_t) * ((*global)->info.n_philo + 1));
    pthread_mutex_init((*global)->mutex, NULL);
    while (++i != (*global)->info.n_philo) 
        make_philo(global, i);
    pthread_create(&((*global)->philos_threads[i]), NULL, &mind, global);
    i = -1;
    while (++i != (*global)->info.n_philo) 
        pthread_join((*global)->philos_threads[i], NULL);
    pthread_join((*global)->philos_threads[i], NULL);
}

// Function to free all allocated memory
static void free_all(t_global **global)
{
    int i;

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

// Main function
int main(int ac, char **av) {
    if ((ac < 5 || ac > 6) && !check_args(av))
    {
        return 0;
    }
    else
    {
        t_global *global = malloc(sizeof(t_global));
        if (!global)
        {
            printf("Error: Memory allocation failed\n");
            return 1;
        }
        init_global_info(&global, ac, av);
        create_philosophers(&global);
        free_all(&global);
    }
    return 0;
}