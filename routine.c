/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:44 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/27 20:30:59 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *msg)
{
	/* Serialize output to avoid interleaved log lines across threads. */
	pthread_mutex_lock(&philo->data->print);
	/* Skip printing once simulation ended to avoid post-death noise. */
	if (!is_dead(philo->data))
	{
		printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id,
			msg);
	}
	pthread_mutex_unlock(&philo->data->print);
}

void	eat(t_philo *philo)
{
	/* Lock both forks before eating to model exclusive fork ownership. */
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	/* Protect meal bookkeeping with the per-philosopher state mutex. */
	pthread_mutex_lock(&philo->death);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->death);
	/* Eating duration is interruptible if monitor marks simulation as dead. */
	smart_sleep(philo->data->time_eat, philo->data);
	/* Release forks so neighbors can continue their routines. */
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	/* Special case: one philosopher can only take one fork, then starves. */
	if (philo->data->nb_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		/* No second fork exists,
			so this philosopher waits until death timeout. */
		smart_sleep(philo->data->time_die, philo->data);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	/* Small stagger reduces immediate lock contention at startup. */
	if (philo->id % 2 == 0)
		usleep(1000);
	/* Main philosopher loop: eat, sleep, think until simulation stops. */
	while ((!is_dead(philo->data)))
	{
		eat(philo);
		print_status(philo, "is sleeping");
		smart_sleep(philo->data->time_sleep, philo->data);
		/* Thinking has no fixed duration here; next loop iteration continues. */
		print_status(philo, "is thinking");
	}
	return (NULL);
}

int	is_dead(t_data *data)
{
	int	d;

	/* Guard shared stop flag with a mutex for thread-safe reads. */
	pthread_mutex_lock(&data->death);
	d = data->dead;
	pthread_mutex_unlock(&data->death);
	/* Return snapshot value so callers can decide whether to continue work. */
	return (d);
}

void	set_dead(t_data *data)
{
	/* One writer path to signal global simulation termination. */
	pthread_mutex_lock(&data->death);
	data->dead = 1;
	pthread_mutex_unlock(&data->death);
}