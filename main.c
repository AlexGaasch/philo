/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:36 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/28 21:54:12 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_positive_long(const char *str, long *out)
{
	int		i;
	long	value;

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

	if (argc != 5 && argc != 6)
		return (0);
	if (!parse_positive_long(argv[1], &nb_philo)
		|| !parse_positive_long(argv[2], &data->time_die)
		|| !parse_positive_long(argv[3], &data->time_eat)
		|| !parse_positive_long(argv[4], &data->time_sleep))
		return (0);
	data->nb_philo = (int)nb_philo;
	data->must_eat = -1;
	if (argc == 6)
	{
		if (!parse_positive_long(argv[5], &must_eat))
			return (0);
		data->must_eat = (int)must_eat;
	}
	return (1);
}

int	free_destroy(t_data data)
{
	int	i;

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
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;
	int			i;

	if (!parse_args(argc, argv, &data))
		return (printf("Error\n"), 1);
	data.dead = 0;
	if (!init_forks(&data) || !init_philos(&data))
		return (1);
	data.start_time = get_time();
	i = -1;
	while (++i < data.nb_philo)
	{
		data.philos[i].last_meal = data.start_time;
		if (pthread_create(&data.philos[i].thread, NULL, routine,
				&data.philos[i]) != 0)
			return (1);
	}
	if (pthread_create(&monitor_thread, NULL, monitor, &data) != 0)
		return (1);
	i = -1;
	while (++i < data.nb_philo)
		pthread_join(data.philos[i].thread, NULL);
	pthread_join(monitor_thread, NULL);
	return (free_destroy(data));
}
