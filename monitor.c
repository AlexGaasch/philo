#include "philo.h"

int all_ate_enough(t_data *data)
{
    int i = 0;
    int meals;

    if (data->must_eat == -1)
        return 0;

    while (i < data->nb_philo)
    {
        pthread_mutex_lock(&data->philos[i].death);
        meals = data->philos[i].meals_eaten;
        pthread_mutex_unlock(&data->philos[i].death);
        if (meals < data->must_eat)
            return 0;
        i++;
    }
    return 1;
}

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;
    long last_meal;

    while (!is_dead(data))
    {
        if (all_ate_enough(data))
        {
            set_dead(data);
            return NULL;
        }
        i = 0;
        while (i < data->nb_philo)
        {
            pthread_mutex_lock(&data->philos[i].death);
            last_meal = data->philos[i].last_meal;
            pthread_mutex_unlock(&data->philos[i].death);
            if (get_time() - last_meal
                > data->time_die)
            {
                pthread_mutex_lock(&data->print);
                printf("%ld %d died\n",
                    get_time() - data->start_time,
                    data->philos[i].id);
                set_dead(data);
                pthread_mutex_unlock(&data->print);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
    return NULL;
}