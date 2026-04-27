*This project has been created as part of the 42 curriculum by agaasch.*

## Description

Philo is a multithreaded simulation of the classic Dining Philosophers problem.
The goal is to coordinate several philosopher threads competing for shared forks
while avoiding deadlocks, race conditions, and incorrect timing behavior.

This project focuses on thread synchronization with mutexes, monitoring shared
state safely, and stopping the simulation when a philosopher dies or when every
philosopher has eaten the required number of meals.

## Instructions

### Installation

Clone the repository from GitHub and enter the project directory:

```bash
git clone https://github.com/AlexGaasch/philo.git
cd philo
```

### Compilation

Build the program with:

```bash
make
```

The project is compiled with `cc` using the flags `-Wall -Wextra -Werror -pthread`.

### Cleanup

Remove object files with:

```bash
make clean
```

Remove the executable and object files with:

```bash
make fclean
```

Rebuild everything with:

```bash
make re
```

### Execution

Run the program with:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Arguments:

* `number_of_philosophers`: number of philosophers and forks.
* `time_to_die`: time in milliseconds before a philosopher dies if they do not eat.
* `time_to_eat`: time in milliseconds spent eating.
* `time_to_sleep`: time in milliseconds spent sleeping.
* `number_of_times_each_philosopher_must_eat`: optional limit that stops the simulation once every philosopher has eaten enough.

Example:

```bash
./philo 5 800 200 200 7
```

### Allowed functions (42)

This implementation is restricted to the following functions:

* memset
* printf
* malloc
* free
* write
* usleep
* gettimeofday
* pthread_create
* pthread_detach
* pthread_join
* pthread_mutex_init
* pthread_mutex_destroy
* pthread_mutex_lock
* pthread_mutex_unlock

## Resources

* Dining Philosophers problem: https://en.wikipedia.org/wiki/Dining_philosophers_problem
* Philosophers 42 Explained / Walktrough: https://github.com/DeRuina/philosophers
* POSIX threads overview: https://man7.org/linux/man-pages/man7/pthreads.7.html
* `pthread_mutex_lock` reference: https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html
* `pthread_create` reference: https://man7.org/linux/man-pages/man3/pthread_create.3.html
* 42 project subject and course notes for the Philosophers project
* Youtube guide to threads / race conditions / mutex: https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=1

AI usage: AI was used to generate this README and to get additional
information on how to use pthread mutexes.
