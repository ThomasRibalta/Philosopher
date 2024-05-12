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
    struct timeval current_time;
    long long millis;


    gettimeofday(&current_time, NULL);
    pthread_mutex_lock(philo->mutex);
    millis = ((current_time.tv_sec * 1000LL) + (current_time.tv_usec / 1000))
     - ((philo->info.start_time.tv_sec * 1000LL) + (philo->info.start_time.tv_usec / 1000));
    pthread_mutex_unlock(philo->mutex);
    pthread_mutex_lock(philo->global);
    philo->state = state;
    if (state == 0)
        printf("%lldms Identifiant du philosophe : %d - pense\n", millis, philo->id);
    else if (state == 1)
        printf("%lldms Identifiant du philosophe : %d - mange\n", millis, philo->id);
    else if (state == 2)
        printf("%lldms Identifiant du philosophe : %d - dort\n", millis, philo->id);
    else if (state == 3)
        printf("%lldms Identifiant du philosophe : %d - est mort\n", millis, philo->id);
    pthread_mutex_unlock(philo->global);
} 

static void init_global_info(t_global **global, int ac, char **av)
{
  t_info info;

  info.n_philo = ft_atoi(av[1]);
  info.time_die = ft_atoi(av[2]);
  info.time_eat = ft_atoi(av[3]);
  info.time_sleep = ft_atoi(av[4]);
  gettimeofday(&(info.start_time), NULL);
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
    if (philo->is_dead == true)
        return ((void) pthread_mutex_unlock(philo->fork + philo->id), (void) pthread_mutex_unlock(philo->fork + philo->id + 1));
}

static void put_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->fork + philo->id);
    if (philo->id == philo->info.n_philo - 1)
    {
        pthread_mutex_unlock(philo->fork);
    }
    else
    {
        pthread_mutex_unlock(philo->fork + philo->id + 1);
    }
}

static void philo_think(t_philo *philo)
{
    if (philo->is_dead == true)
      return;
    set_state(philo, 0);
    return;
}

static void philo_sleep(t_philo *philo)
{
    if (philo->is_dead == true)
      return;
    set_state(philo, 2);
    usleep(philo->info.time_sleep * 1000);
    philo_think(philo);
}

static void philo_eat(t_philo *philo)
{
  pthread_mutex_lock(philo->mutex);
  gettimeofday(&(philo->last_time_eat), NULL);
  pthread_mutex_unlock(philo->mutex);
  if (philo->is_dead == true)
    return;
  set_state(philo, 1);
  usleep(philo->info.time_eat * 1000);
  put_forks(philo);
  philo_sleep(philo);
}

static void *test(void *arg){
    t_philo *philo;
    
    philo = (t_philo *)arg;
    while (philo->is_dead == false){
        take_forks(philo);
        philo_eat(philo);
    }
    return NULL;
}

static bool one_philo_is_death(t_global **global)
{
    int i;
    struct timeval current_time;
    long long millis;

    i = -1;
    while (++i != (*global)->info.n_philo)
    {
        gettimeofday(&current_time, NULL);
        pthread_mutex_lock((*global)->philos[i].mutex);
        millis = ((current_time.tv_sec * 1000LL) + (current_time.tv_usec / 1000))
         - (((*global)->philos[i].last_time_eat.tv_sec * 1000LL) + ((*global)->philos[i].last_time_eat.tv_usec / 1000));
        pthread_mutex_unlock((*global)->philos[i].mutex);
        if (millis > (*global)->info.time_die)
        {
            pthread_mutex_lock((*global)->philos[i].global);
            (*global)->philos[i].is_dead = true;
            pthread_mutex_unlock((*global)->philos[i].global);
            set_state(&(*global)->philos[i], 3);
            return (true);
        }
    }
    return (false);
}

static void kill_all_philos(t_global **global)
{
    int i;

    i = -1;
    while (++i != (*global)->info.n_philo)
    {
        pthread_mutex_lock((*global)->philos[i].global);
        (*global)->philos[i].is_dead = true;
        pthread_mutex_unlock((*global)->philos[i].global);
    }
}

static void *mind(void *arg)
{
    t_global **global;

    global = (t_global **)arg;
    while (1)
    {
        if (one_philo_is_death(global))
        {
          kill_all_philos(global);
          return (NULL);
        }
    }
    return (NULL);
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
  (*global)->philo_mutex = malloc(sizeof(pthread_mutex_t) * (*global)->info.n_philo);
  if (!(*global)->philo_mutex)
      return (free((*global)->philos), free((*global)->mutex));
  (*global)->fork = malloc(sizeof(pthread_mutex_t) * (*global)->info.n_philo);
  pthread_mutex_init((*global)->mutex, NULL);
  (*global)->philos_threads = malloc(sizeof(pthread_t) * (*global)->info.n_philo);
  if (!(*global)->philos_threads)
      return (free((*global)->philos), free((*global)->mutex));
  while (++i != (*global)->info.n_philo) {
      pthread_mutex_init((*global)->philo_mutex + i, NULL);
      (*global)->philos[i].info = (*global)->info;
      (*global)->philos[i].mutex = (*global)->philo_mutex + i;
      (*global)->philos[i].id = i;
      (*global)->philos[i].global = (*global)->mutex;
      (*global)->philos[i].is_dead = false;
      (*global)->philos[i].last_time_eat = (*global)->info.start_time;
      pthread_mutex_init((*global)->fork + i, NULL);
  }
  i = -1;
  while (++i != (*global)->info.n_philo){
    (*global)->philos[i].fork = (*global)->fork;
      pthread_create(&((*global)->philos_threads[i]), NULL, &test, &(*global)->philos[i]);
  }
  pthread_create(&((*global)->philos_threads[i]), NULL, &mind, global);
  i = -1;
  while (++i != (*global)->info.n_philo)
      pthread_join((*global)->philos_threads[i], NULL);
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