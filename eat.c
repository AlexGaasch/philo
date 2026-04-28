/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:02:34 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/28 19:16:36 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	get_forks(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}

static int	take_forks(t_philo *philo, pthread_mutex_t *first,
		pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	print_status(philo, "has taken a fork");
	if (is_dead(philo->data))
	{
		pthread_mutex_unlock(first);
		return (0);
	}
	pthread_mutex_lock(second);
	print_status(philo, "has taken a fork");
	if (is_dead(philo->data))
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);
		return (0);
	}
	return (1);
}

static int	do_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->death);
	if (get_time() - philo->last_meal > philo->data->time_die)
	{
		pthread_mutex_unlock(&philo->death);
		return (0);
	}
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->death);
	print_status(philo, "is eating");
	smart_sleep(philo->data->time_eat, philo->data);
	philo->meals_eaten++;
	return (1);
}

int	eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (is_dead(philo->data))
		return (0);
	get_forks(philo, &first, &second);
	if (!take_forks(philo, first, second))
		return (0);
	if (!do_eat(philo))
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);
		return (0);
	}
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
	return (1);
}
