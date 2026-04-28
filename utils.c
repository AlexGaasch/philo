/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:47 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/28 21:39:20 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	put_number(char *buf, long nb)
{
	int		i;
	char	tmp[20];
	int		j;

	i = 0;
	if (nb == 0)
	{
		buf[i++] = '0';
		return (i);
	}
	if (nb < 0)
	{
		buf[i++] = '-';
		nb = -nb;
	}
	j = 0;
	while (nb > 0)
	{
		tmp[j++] = (nb % 10) + '0';
		nb /= 10;
	}
	while (j--)
		buf[i++] = tmp[j];
	return (i);
}

int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (0);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (1);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (0);
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].meals_eaten = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philo];
		pthread_mutex_init(&data->philos[i].death, NULL);
		data->philos[i].data = data;
		i++;
	}
	return (1);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	smart_sleep(long time, t_data *data)
{
	long	start;

	start = get_time();
	while (!is_dead(data))
	{
		if (get_time() - start >= time)
			break ;
		usleep(100);
	}
}
