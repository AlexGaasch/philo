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
	return (1);
}
static void	do_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->death);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->death);
	print_status(philo, "is eating");
	smart_sleep(philo->data->time_eat, philo->data);
	philo->meals_eaten++;
}

void	eat(t_philo *philo)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;

	if (is_dead(philo->data))
		return ;
	get_forks(philo, &first, &second);
	if (!take_forks(philo, first, second))
		return ;
	do_eat(philo);
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
}