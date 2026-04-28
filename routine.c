/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:44 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/28 13:45:58 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static int	put_number(char *buf, long nb)
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

void	print_status(t_philo *philo, char *msg)
{
	char	buf[128];
	int		i;
	long	time;
	int		j;

	pthread_mutex_lock(&philo->data->print);
	if (!is_dead(philo->data))
	{
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
	}
	pthread_mutex_unlock(&philo->data->print);
}

void	eat(t_philo *philo)
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

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		smart_sleep(philo->data->time_die, philo->data);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
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
	return (NULL);
}
