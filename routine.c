#include "philo.h"

void print_status(t_philo *philo, char *msg)
{
    pthread_mutex_lock(&philo->data->print);

    if (!is_dead(philo->data))
    {
        printf("%ld %d %s\n",
            get_time() - philo->data->start_time,
            philo->id, msg);
    }

    pthread_mutex_unlock(&philo->data->print);
}

void eat(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_status(philo, "has taken a fork");

    pthread_mutex_lock(philo->right_fork);
    print_status(philo, "has taken a fork");

    print_status(philo, "is eating");
    pthread_mutex_lock(&philo->death);
    philo->last_meal = get_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->death);
    smart_sleep(philo->data->time_eat, philo->data);

    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if (philo->data->nb_philo == 1)
    {
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
        smart_sleep(philo->data->time_die, philo->data);
        pthread_mutex_unlock(philo->left_fork);
        return NULL;
    }

    if (philo->id % 2 == 0)
        usleep(1000);

    while ((!is_dead(philo->data)))
    {
        eat(philo);
        print_status(philo, "is sleeping");
        smart_sleep(philo->data->time_sleep, philo->data);
        print_status(philo, "is thinking");
    }
    return NULL;
}

int is_dead(t_data *data)
{
    int d;

    pthread_mutex_lock(&data->death);
    d = data->dead;
    pthread_mutex_unlock(&data->death);
    return d;
}

void set_dead(t_data *data)
{
    pthread_mutex_lock(&data->death);
    data->dead = 1;
    pthread_mutex_unlock(&data->death);
}