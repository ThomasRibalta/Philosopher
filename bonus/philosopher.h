#ifndef PHILOSOPHER
# define PHILOSOPHER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

typedef struct s_philo
{
  struct timeval last_time_eat;
  struct s_info *info;
  pid_t phil_fork_id;
  int n_eat;
  int id;
  int state;
} t_philo;

typedef struct s_info
{
  int n_philo;
  int time_die;
  int time_eat;
  int time_sleep;
  int eat_interval;
  sem_t *forks;
  sem_t *writing;
  t_philo *philos;
  struct timeval start_time;
} t_info;

// 0 = thinking
// 1 = eating
// 2 = sleeping
// 3 = fork
// 4 = dead

int ft_atoi(const char *str);
void ft_usleep(long int time_in_ms);
long long get_time(void);

#endif 
