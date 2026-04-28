/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:44 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/28 20:12:25 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

void	print_status(t_philo *philo, char *msg)
{
	char	buf[128];
	int		i;
	long	time;
	int		j;

	pthread_mutex_lock(&philo->data->death);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->death);
		return ;
	}
	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_unlock(&philo->data->death);
	i = 0;
	time = get_time() - philo->data->start_time;
	i += put_number(buf + i, time);
	buf[i++] = ' ';
	i += put_number(buf + i, philo->id);
	buf[i++] = ' ';
	j = 0;
	while (msg[j])
		buf[i++] = msg[j++];
	buf[i++] = '\n';
	write(1, buf, i);
	pthread_mutex_unlock(&philo->data->print);
}

void	*handle_single(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	smart_sleep(philo->data->time_die, philo->data);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->death);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->death);
	if (philo->data->nb_philo == 1)
		return (handle_single(philo));
	if (philo->id % 2 == 0)
		usleep(1000);
	while ((!is_dead(philo->data)))
	{
		if (!eat(philo))
			break ;
		print_status(philo, "is sleeping");
		smart_sleep(philo->data->time_sleep, philo->data);
		print_status(philo, "is thinking");
		usleep(500);
	}
	return (NULL);
}
