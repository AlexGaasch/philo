/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:39 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/28 14:28:14 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_data *data)
{
	int	d;

	pthread_mutex_lock(&data->death);
	d = data->dead;
	pthread_mutex_unlock(&data->death);
	return (d);
}

void	set_dead(t_data *data)
{
	pthread_mutex_lock(&data->death);
	data->dead = 1;
	pthread_mutex_unlock(&data->death);
}

int	all_ate_enough(t_data *data)
{
	int		i;
	int		meals;

	i = 0;
	if (data->must_eat == -1)
		return (0);
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->philos[i].death);
		meals = data->philos[i].meals_eaten;
		pthread_mutex_unlock(&data->philos[i].death);
		if (meals < data->must_eat)
			return (0);
		i++;
	}
	return (1);
}

void	*died_to_time(t_data *data, int i)
{
	set_dead(data);
	pthread_mutex_lock(&data->print);
	printf("%ld %d died\n", get_time() - data->start_time, data->philos[i].id);
	pthread_mutex_unlock(&data->print);
	return (NULL);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	long	last_meal;

	data = (t_data *)arg;
	while (!is_dead(data))
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->philos[i].death);
			last_meal = data->philos[i].last_meal;
			pthread_mutex_unlock(&data->philos[i].death);
			if (get_time() - last_meal > data->time_die)
				return (died_to_time(data, i));
			i++;
		}
		if (all_ate_enough(data))
		{
			set_dead(data);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
