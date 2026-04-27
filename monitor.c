/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:39 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/27 20:30:14 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int all_ate_enough(t_data *data)
{
    int i = 0;
    int meals;

    /* No meal target means this completion condition is disabled. */
    if (data->must_eat == -1)
        return 0;

    while (i < data->nb_philo)
    {
        /* meals_eaten is shared with the routine thread, so lock around read. */
        pthread_mutex_lock(&data->philos[i].death);
        meals = data->philos[i].meals_eaten;
        pthread_mutex_unlock(&data->philos[i].death);
        /* Stop early if any philosopher has not reached the target. */
        if (meals < data->must_eat)
            return 0;
        i++;
    }
    return 1;
}
void	*died_to_time(t_data *data, int i)
{
	pthread_mutex_lock(&data->print);
	/* Print death before setting stop flag to keep output deterministic. */
	printf("%ld %d died\n", get_time() - data->start_time, data->philos[i].id);
	set_dead(data);
	pthread_mutex_unlock(&data->print);
	return (NULL);
}

void *monitor(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;
    long last_meal;

    while (!is_dead(data))
    {
        /* End simulation once everyone reached the required meals. */
        if (all_ate_enough(data))
        {
            set_dead(data);
            return NULL;
        }
        i = 0;
        /* Scan each philosopher to detect starvation as soon as possible. */
        while (i < data->nb_philo)
        {
            /* last_meal is updated by eat(), so this read must be synchronized. */
            pthread_mutex_lock(&data->philos[i].death);
            last_meal = data->philos[i].last_meal;
            pthread_mutex_unlock(&data->philos[i].death);
            /* A philosopher dies if time since last meal exceeds time_to_die. */
            if (get_time() - last_meal > data->time_die)
				return (died_to_time(data, i));
            i++;
        }
        /* Small polling delay prevents busy-waiting while keeping responsiveness. */
        usleep(1000);
    }
    return NULL;
}