#include "philosopher.h"

int	ft_atoi(const char *str)
{
	int	mult;
	int	result;

	result = 0;
	mult = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
	{
		mult *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (mult * result);
}

// Function to set philosopher state
void set_state(t_philo *philo, int state) {
    struct timeval current_time;
    long long millis;

    gettimeofday(&current_time, NULL);
    millis = ((current_time.tv_sec * 1000LL) + (current_time.tv_usec / 1000))
            - ((philo->info.start_time.tv_sec * 1000LL) + (philo->info.start_time.tv_usec / 1000));
    pthread_mutex_lock(philo->global);
    philo->state = state;
    if (state == 0 && philo->is_dead == false)
        printf("%lldms philosophe n^%d thinking\n", millis, philo->id);
    else if (state == 1 && philo->is_dead == false)
        printf("%lldms philosophe n^%d eating (%d)\n", millis, philo->id, philo->n_eat);
    else if (state == 2 && philo->is_dead == false)
        printf("%lldms philosophe n^%d sleeping\n", millis, philo->id);
    else if (state == 3 && philo->is_dead == false)
        printf("%lldms philosophe n^%d has taken a fork\n", millis, philo->id);
    else if (state == 4 && philo->is_dead == true)
        printf("%lldms philosophe n^%d died\n", millis, philo->id);
    pthread_mutex_unlock(philo->global);
}

// Function to simulate thread sleeping
void ft_usleep(long int time_in_ms) {
    long int start_time;
		struct timeval current_time;

    gettimeofday(&current_time, NULL);
    start_time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
    while ((get_time() - start_time) < time_in_ms)
        usleep(time_in_ms / 10);
}

// Function to initialize global information
static void init_global_info(t_global **global, int ac, char **av) {
    t_info info;

    info.n_philo = atoi(av[1]);
    info.time_die = atoi(av[2]);
    info.time_eat = atoi(av[3]);
    info.time_sleep = atoi(av[4]);
    gettimeofday(&(info.start_time), NULL);
    if (ac == 6)
        info.eat_interval = atoi(av[5]);
    else
        info.eat_interval = -1;
    (*global)->info = info;
}

// Function to take forks
static void take_forks(t_philo *philo) {
    if (philo->is_dead == true)
        return;
    pthread_mutex_lock(philo->fork + philo->id);
    pthread_mutex_lock(philo->mutex);
    if (philo->is_dead == true) {
        pthread_mutex_unlock(philo->mutex);
        pthread_mutex_unlock(philo->fork + philo->id);
        return;
    }
    pthread_mutex_unlock(philo->mutex);
    pthread_mutex_lock(philo->fork + ((philo->id + 1) * (philo->id != philo->info.n_philo - 1)));
    pthread_mutex_lock(philo->mutex);
    if (philo->is_dead == true) {
        pthread_mutex_unlock(philo->mutex);
        pthread_mutex_unlock(philo->fork + philo->id);
        pthread_mutex_unlock(philo->fork + ((philo->id + 1) * (philo->id != philo->info.n_philo - 1)));
        return;
    }
    pthread_mutex_unlock(philo->mutex);
    set_state(philo, 3);
}

// Function to put forks
static void put_forks(t_philo *philo) {
    pthread_mutex_unlock(philo->fork + philo->id);
    if (philo->id == philo->info.n_philo - 1)
        pthread_mutex_unlock(philo->fork);
    else
        pthread_mutex_unlock(philo->fork + philo->id + 1);
}