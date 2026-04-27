#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>

typedef struct s_data t_data;

typedef struct s_philo
{
    int             id;
    long            last_meal;
    int             meals_eaten;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t death;
    t_data          *data;
} t_philo;

typedef struct s_data
{
    int             nb_philo;
    long            time_die;
    long            time_eat;
    long            time_sleep;
    int             must_eat;

    long            start_time;
    int             dead;

    pthread_mutex_t *forks;
    pthread_mutex_t print;
    pthread_mutex_t death;

    t_philo         *philos;
} t_data;

void init_forks(t_data *data);
void init_philos(t_data *data);
long get_time(void);
void smart_sleep(long time, t_data *data);
void print_status(t_philo *philo, char *msg);
void eat(t_philo *philo);
void *routine(void *arg);
void *monitor(void *arg);
int is_dead(t_data *data);
void set_dead(t_data *data);

#endif