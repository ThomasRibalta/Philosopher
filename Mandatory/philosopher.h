#ifndef PHILOSOPHER
# define PHILOSOPHER

#include <stdio.h>
#include <unistd.h>

typedef struct s_info
{
  int n_philo;
  int time_die;
  int time_eat;
  int time_sleep;
  int eat_interval;
} t_info;


int	ft_atoi(const char *str);

#endif 
