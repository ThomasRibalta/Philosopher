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

long int	get_time(void)
{
	long int			time;
	struct timeval		current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
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
  else
    info.eat_interval = -1;
  (*global)->info = info;
  }

static void take_forks(t_philo *philo)
{
  if (philo->is_dead == true)
    return;
  pthread_mutex_lock(philo->fork + philo->id);
  pthread_mutex_lock(philo->mutex);
  if (philo->is_dead == true)
    return ((void) pthread_mutex_unlock(philo->mutex), (void) pthread_mutex_unlock(philo->fork + philo->id));
  pthread_mutex_unlock(philo->mutex);
  pthread_mutex_lock(philo->fork + ((philo->id + 1) * (philo->id != philo->info.n_philo - 1)));
  pthread_mutex_lock(philo->mutex);
  if (philo->is_dead == true)
    return ((void) pthread_mutex_unlock(philo->fork + philo->id), (void) pthread_mutex_unlock(philo->fork + ((philo->id + 1) * (philo->id != philo->info.n_philo - 1))), (void) pthread_mutex_unlock(philo->mutex));
  pthread_mutex_unlock(philo->mutex);
  set_state(philo, 3);
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
    ft_usleep(philo->info.time_sleep);
    philo_think(philo);
}

static void philo_eat(t_philo *philo)
{
  set_state(philo, 1);
  pthread_mutex_lock(philo->mutex);
  gettimeofday(&(philo->last_time_eat), NULL);
  pthread_mutex_unlock(philo->mutex);
  if (philo->is_dead == true)
    return;
  ft_usleep(philo->info.time_eat);
  pthread_mutex_lock(philo->mutex);
  gettimeofday(&(philo->last_time_eat), NULL);
  philo->n_eat++;
  pthread_mutex_unlock(philo->mutex);
  put_forks(philo);
  philo_sleep(philo);
}

static void *test(void *arg){
    t_philo *philo;
    
    philo = (t_philo *)arg;
    philo->last_time_eat = philo->info.start_time;
    while (philo->is_dead == false){
      if (philo->info.n_philo > 1)
      {
          take_forks(philo);
          philo_eat(philo);
      }
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
        millis = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000))
         - (((*global)->philos[i].last_time_eat.tv_sec * 1000) + ((*global)->philos[i].last_time_eat.tv_usec / 1000));
        if (millis == (*global)->info.time_die && (*global)->philos[i].state != 1)
        {
            pthread_mutex_lock((*global)->philos[i].global);
            (*global)->philos[i].is_dead = true;
            pthread_mutex_unlock((*global)->philos[i].global);
            set_state(&(*global)->philos[i], 4);
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
        (*global)->philos[i].is_dead = true;
    }
}

static bool end_eat_interval(t_global **global)
{
    int i;
    struct timeval current_time;
    long long millis;
    bool result;

    i = -1;
    while (++i != (*global)->info.n_philo)
    {
        if ((*global)->philos[i].n_eat >= (*global)->info.eat_interval && (*global)->info.eat_interval != -1)
        {
            result = true;
        }
        else
        {
            result = false;
            break;
        }
    }
    return (result);
}

static void *mind(void *arg)
{
    t_global **global;

    global = (t_global **)arg;
    while (1)
    {
        if (one_philo_is_death(global) || end_eat_interval(global))
        {
          pthread_mutex_lock((*global)->mutex);
          kill_all_philos(global);
          pthread_mutex_unlock((*global)->mutex);
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
      (*global)->philos[i].n_eat = 0;
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

static void free_all(t_global **global)
{
  int i;

  i = -1;
  while (++i != (*global)->info.n_philo)
  {
    pthread_mutex_destroy((*global)->philo_mutex + i);
    pthread_mutex_destroy((*global)->fork + i);
  }
  pthread_mutex_destroy((*global)->philo_mutex + i);
  pthread_mutex_destroy((*global)->mutex);
  free((*global)->philos);
  free((*global)->mutex);
  free((*global)->fork);
  free((*global)->philo_mutex);
  free((*global)->philos_threads);
  free(*global);
}

static void start(int ac, char **av)
{
  t_global *global;
  global = malloc(sizeof(t_global));
  if (!global)
    return((void) printf("erreur malloc"), (void) 0);
  init_global_info(&global, ac, av);
  create_philosophers(&global);
  free_all(&global);
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