/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:47 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/27 20:31:02 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_forks(t_data *data)
{
	int	i;

	i = 0;
	/* Shared mutexes: one per fork + global print/death locks. */
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	while (i < data->nb_philo)
	{
		/* Each fork mutex represents exclusive access to one physical fork. */
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	while (i < data->nb_philo)
	{
		/* Philosopher IDs are 1-based to match expected output format. */
		data->philos[i].id = i + 1;
		/* At start, everyone is considered to have just eaten at start_time. */
		data->philos[i].last_meal = data->start_time;
		data->philos[i].meals_eaten = 0;
		/* Each philosopher takes fork i and fork (i + 1) mod N. */
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philo];
		/* Per-philosopher mutex protects last_meal and meals_eaten fields. */
		pthread_mutex_init(&data->philos[i].death, NULL);
		data->philos[i].data = data;
		i++;
	}
}

long	get_time(void)
{
	struct timeval	tv;

	/* Return current wall-clock time in milliseconds. */
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	smart_sleep(long time, t_data *data)
{
	long start = get_time();

	/* Sleep in short slices so threads can react quickly to stop events. */
	while (!is_dead(data))
	{
		/* Exit once the requested duration elapsed since entering sleep. */
		if (get_time() - start >= time)
			break ;
		/* 100us granularity balances CPU usage and wake-up responsiveness. */
		usleep(100);
	}
}