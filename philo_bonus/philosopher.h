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
#include <signal.h>
#include <sys/types.h>

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
  int end_process;
  sem_t *forks;
  sem_t *writing;
  sem_t *eat;
  t_philo *philos;
  struct timeval start_time;
} t_info;

int ft_atoi(const char *str);
void ft_usleep(long int time_in_ms);
static void philo_eat(t_philo *philo);
void *mind(void *arg);
void kill_all(t_info *info);
void free_all(t_info *info);
void set_state(t_philo *philo, int state);
void philosopher_behavior(t_philo *philo);
void put_long_long(long long n);
long long get_time(void);

#endif 
