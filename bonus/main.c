#include "philosopher.h"

// Function to simulate thread sleeping
void ft_usleep(long int time_in_ms)
{
    long int start_time;
		struct timeval current_time;

    gettimeofday(&current_time, NULL);
    start_time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
    while ((get_time() - start_time) < time_in_ms)
        usleep(time_in_ms / 10);
}

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

long long get_time(void)
{
    struct timeval current_time;

    gettimeofday(&current_time, NULL);
    return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
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
	info->forks = sem_open("/forks", O_CREAT, S_IRWXU, info->n_philo);
	info->writing = sem_open("/write", O_CREAT, S_IRWXU, 1);
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

static void putchar2(char c)
{
    write(1, &c, 1);
}

static void put_long_long(long long n)
{
    if (n > 9)
        put_long_long(n / 10);
    putchar2(n % 10 + '0');
}

static void set_state(t_philo *philo, int state)
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
        write(1, " died\n", 6);
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

static void philo_eat(t_philo *philo)
{
    set_state(philo, 1);
    gettimeofday(&(philo->last_time_eat), NULL);
    ft_usleep(philo->info->time_eat);
    philo->n_eat++;
    put_forks(philo);
    set_state(philo, 2);
    ft_usleep(philo->info->time_sleep);
    set_state(philo, 0);
}

static void philosopher_behavior(t_philo *philo)
{
    while (1)
    {
        take_forks(philo);
        philo_eat(philo);
    }
}

static void launch_processes(t_info *info)
{
    int i;

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
        waitpid(info->philos[i].phil_fork_id, NULL, 0);
}

// Main function
int main(int ac, char **av) {
    if ((ac < 5 || ac > 6) && !check_args(av))
    {
        return 0;
    }
    else
    {
        t_info *info;

        info = malloc(sizeof(t_info));
        if (!info)
        {
            printf("Error: Memory allocation failed\n");
            return 1;
        }
        init_global_info(info, ac, av);
        init_philos(info);
        launch_processes(info);
    }
    return 0;
}