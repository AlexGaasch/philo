/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:36 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/27 20:30:25 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_positive_long(const char *str, long *out)
{
    int						i;
    long					value;

    /* Accept only strictly positive integer arguments made of digits. */
    if (!str || !*str)
        return (0);
    value = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        value = value * 10 + (str[i] - '0');
        if (value <= 0)
            return (0);
        i++;
    }
    *out = value;
    return (1);
}

static int	parse_args(int argc, char **argv, t_data *data)
{
    long	nb_philo;
    long	must_eat;

    /* Parse required timings; optional meal limit stays disabled at -1. */
    if (argc != 5 && argc != 6)
        return (0);
    /* argv[1..4] are mandatory and must be valid positive numbers. */
    if (!parse_positive_long(argv[1], &nb_philo)
        || !parse_positive_long(argv[2], &data->time_die)
        || !parse_positive_long(argv[3], &data->time_eat)
        || !parse_positive_long(argv[4], &data->time_sleep))
        return (0);
    data->nb_philo = (int)nb_philo;
    data->must_eat = -1;
    if (argc == 6)
    {
        /* Optional stop condition: everybody must eat at least must_eat times. */
        if (!parse_positive_long(argv[5], &must_eat))
            return (0);
        data->must_eat = (int)must_eat;
    }
    return (1);
}

void	free_destroy(t_data data)
   /* Destroy all mutexes and release heap allocations on shutdown. */
{
	int		i;

	i = 0;
	while (i < data.nb_philo)
	{
		pthread_mutex_destroy(&data.forks[i]);
		pthread_mutex_destroy(&data.philos[i].death);
		i++;
	}
	pthread_mutex_destroy(&data.print);
	pthread_mutex_destroy(&data.death);
	free(data.forks);
	free(data.philos);
}

int main(int argc, char **argv)
{
    t_data data;
    pthread_t monitor_thread;
    int i;

    if (!parse_args(argc, argv, &data))
        return (printf("Error\n"), 1);
    data.dead = 0;
    /* All timestamps are printed relative to this shared start point. */
    data.start_time = get_time();

    /* Initialize shared resources before launching any worker thread. */
    init_forks(&data);
    init_philos(&data);

    i = 0;
    while (i < data.nb_philo)
    {
        /* One thread per philosopher running the life cycle routine. */
        pthread_create(&data.philos[i].thread,
            NULL, routine, &data.philos[i]);
        i++;
    }

    /* Dedicated watcher thread checks death and completion conditions. */
    pthread_create(&monitor_thread, NULL, monitor, &data);

    i = 0;
    /* Wait for all philosopher routines to finish before cleanup. */
    while (i < data.nb_philo)
        pthread_join(data.philos[i++].thread, NULL);

    /* Monitor exits once a death/completion condition is detected. */
    pthread_join(monitor_thread, NULL);
	free_destroy(data);
    return 0;
}


/// 50 410 200 100 10