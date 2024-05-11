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

static void set_state(t_philo *philo, int state)
{
    philo->state = state;
    if (state == 0)
        printf("Identifiant du philosophe : %d - pense\n", philo->id);
    else if (state == 1)
        printf("Identifiant du philosophe : %d - mange\n", philo->id);
    else if (state == 2)
        printf("Identifiant du philosophe : %d - dort\n", philo->id);
    else if (state == 3)
        printf("Identifiant du philosophe : %d - est mort\n", philo->id);
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

static void take_forks(t_philo *philo)
{
    if (philo->id == philo->info.n_philo - 1)
    {
        pthread_mutex_lock(philo->fork);
        //printf("Identifiant du philosophe : %d - prend fourchette n^%d\n", philo->id, 0);
        pthread_mutex_lock(philo->fork + philo->id);
        //printf("Identifiant du philosophe : %d - prend fourchette n^%d\n", philo->id, philo->id);
    }
    else
    {
        pthread_mutex_lock(philo->fork + philo->id);
        //printf("Identifiant du philosophe : %d - prend fourchette n^%d\n", philo->id, philo->id);
        pthread_mutex_lock(philo->fork + philo->id + 1);
        //printf("Identifiant du philosophe : %d - prend fourchette n^%d\n", philo->id, philo->id + 1);
    }
    set_state(philo, 1);
}

static void put_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->fork + philo->id);
    if (philo->id == philo->info.n_philo - 1)
    {
        pthread_mutex_unlock(philo->fork);
        //printf("Identifiant du philosophe : %d - pose fourchette n^%d\n", philo->id, 0);
    }
    else
    {
        pthread_mutex_unlock(philo->fork + philo->id + 1);
        //printf("Identifiant du philosophe : %d - pose fourchette n^%d\n", philo->id, philo->id + 1);
    }
}

static void *test(void *arg){
    t_philo *philo;
    
    philo = (t_philo *)arg;

    take_forks(philo);
    put_forks(philo);
    return NULL;
}

static void create_philosophers(t_global **global){
  int i;

  i = -1;
  (*global)->philos = malloc(sizeof(t_philo) * (*global)->info.n_philo);
  if (!(*global)->philos)
      return;
  (*global)->mutex = malloc(sizeof(pthread_mutex_t));
  if (!(*global)->mutex)
      return (free((*global)->philos));
  (*global)->fork = malloc(sizeof(pthread_mutex_t) * (*global)->info.n_philo);
  pthread_mutex_init((*global)->mutex, NULL);
  (*global)->philos_threads = malloc(sizeof(pthread_t) * (*global)->info.n_philo);
  if (!(*global)->philos_threads)
      return (free((*global)->philos), free((*global)->mutex));
  while (++i != (*global)->info.n_philo) {
      (*global)->philos[i].info = (*global)->info;
      (*global)->philos[i].id = i;
      (*global)->philos[i].global = (*global)->mutex;
      set_state(&(*global)->philos[i], 0);
      pthread_mutex_init((*global)->fork + i, NULL);
  }
  i = -1;
  while (++i != (*global)->info.n_philo){
    (*global)->philos[i].fork = (*global)->fork;
      pthread_create(&((*global)->philos_threads[i]), NULL, &test, &(*global)->philos[i]);
  }
  i = -1;
  while (++i != (*global)->info.n_philo)
      pthread_join((*global)->philos_threads[i], NULL);
  printf("fin du join\n");
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