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

static void init_global_info(t_global **global, int ac, char **av)
{
  t_info info;

  info.n_philo = ft_atoi(av[1]);
  info.time_die = ft_atoi(av[2]);
  info.time_eat = ft_atoi(av[3]);
  info.time_sleep = ft_atoi(av[4]);
  if (ac == 6)
    info.eat_interval = ft_atoi(av[5]);
  (*global)->info = info;
  }

static void *test(){
  printf("test prev\n");
  usleep(10000000);
  printf("test\n");
  return (NULL);
}

static void create_philosophers(t_global **global){
  int i;

  i = 0;
  (*global)->philos = malloc(sizeof(t_philo) * (*global)->info.n_philo);
  if (!(*global)->philos)
      return;
  (*global)->philos_threads = malloc(sizeof(pthread_t) * (*global)->info.n_philo);
  if (!(*global)->philos_threads)
      return (free((*global)->philos));
  while (i++ != (*global)->info.n_philo) {
      (*global)->philos[i].is_dead = 0;
      (*global)->philos[i].fork = malloc(sizeof(pthread_mutex_t));
      if (!(*global)->philos[i].fork)
        return (free((*global)->philos));
      pthread_mutex_init((*global)->philos[i].fork, NULL);
      pthread_create(&((*global)->philos_threads[i]), NULL, &test, NULL);
  }
  i = 0;
  while (i++ != (*global)->info.n_philo)
      pthread_join((*global)->philos_threads[i], NULL);
}

static void start(int ac, char **av)
{
  t_global *global;
  global = malloc(sizeof(t_global));
  if (!global)
    return((void) printf("erreur malloc"), (void) 0);
  init_global_info(&global, ac, av);
  create_philosophers(&global);
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