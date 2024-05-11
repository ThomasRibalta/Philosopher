#ifndef PHILOSOPHER
# define PHILOSOPHER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_info
{
  int n_philo;
  int time_die;
  int time_eat;
  int time_sleep;
  int eat_interval;
} t_info;

// 0 = thinking
// 1 = eating
// 2 = sleeping
// 3 = dead

typedef struct s_philo
{
  pthread_mutex_t	*fork;
  pthread_mutex_t	*global;
  t_info info;
  int id;
  int state;
} t_philo;

typedef struct s_global
{
  t_info info;
  t_philo *philos;
  pthread_mutex_t	*fork;
  pthread_mutex_t	*mutex;
  pthread_t *philos_threads;

} t_global;


int	ft_atoi(const char *str);

#endif 
