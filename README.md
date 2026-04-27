*This project has been created as part of the 42 curriculum by agaasch.*

## Description

Philo is a multithreaded simulation of the classic Dining Philosophers problem.
The goal is to coordinate several philosopher threads competing for shared forks
while avoiding deadlocks, race conditions, and incorrect timing behavior.

This project focuses on thread synchronization with mutexes, monitoring shared
state safely, and stopping the simulation when a philosopher dies or when every
philosopher has eaten the required number of meals.

## Instructions

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

## Resources

* Dining Philosophers problem: https://en.wikipedia.org/wiki/Dining_philosophers_problem
* POSIX threads overview: https://man7.org/linux/man-pages/man7/pthreads.7.html
* `pthread_mutex_lock` reference: https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html
* `pthread_create` reference: https://man7.org/linux/man-pages/man3/pthread_create.3.html
* 42 project subject and course notes for the Philosophers project

AI usage: AI was used to draft this README structure and to generate ideas for
approaching the project. The code itself was not generated blindly; the final
implementation and wording should be verified against the project source and the
42 requirements.
