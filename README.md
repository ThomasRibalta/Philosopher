# GOAL OF Philosopher
For this project, we have (n > 1) philosopher and (philosopher + 1) forks. All philosopher have one fork on his left. We can imagine all the philosophe sit around a table, the philosophe can only think eat and sleep. When you lunch the program you need to add n_philosophe, time_die, time_eat, time_sleep.
```
./philo n_philo(1,200) time_die(n > 60) time_eat(n > 60) time_sleep(n > 60)
```
The subject asks us to make 1 thread for 1 philosophe, we use mutex on mandatory part for data race. In the bonus part is the same subject just instead of mutexes and threads, we must use forks and semaphore.

# RESULT

![Screenshot from 2023-11-24 22-06-16](https://github.com/mamaPvP/Pipex/assets/105978556/06b7abf6-a87d-49f2-afdb-ee3460e9fa1b)
