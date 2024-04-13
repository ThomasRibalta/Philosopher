#include "philosopher.h"

static int check_args(char **args)
{
  int tmp;

  tmp = 0;
  while (*args){
    if (ft_atoi(*args) <= 0)
      return (0);
    args++;
  }
  return (1);
}

static void init_global_info(t_info *info, int ac, char **av)
{
  info->n_philo = ft_atoi(av[1]);
  info->time_die = ft_atoi(av[2]);
  info->time_eat = ft_atoi(av[3]);
  info->time_sleep = ft_atoi(av[4]);
  if (ac == 6)
    info->eat_interval = ft_atoi(av[5]);
  }

static void start(int ac, char **av)
{
  t_info info;
  init_global_info(&info, ac, av);
  printf("%d", info.n_philo);
}

int main(int ac, char **av)
{
  if ((ac < 5 || ac > 6) && !check_args(av))
  {
    return (0);
  }else
  {
    start(ac, av);
  }
}