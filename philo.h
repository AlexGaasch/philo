/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaasch <agaasch@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 19:40:42 by agaasch           #+#    #+#             */
/*   Updated: 2026/04/28 18:54:00 by agaasch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	long				last_meal;
	int					meals_eaten;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		death;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					nb_philo;
	long				time_die;
	long				time_eat;
	long				time_sleep;
	int					must_eat;

	long				start_time;
	int					dead;

	pthread_mutex_t		*forks;
	pthread_mutex_t		print;
	pthread_mutex_t		death;

	t_philo				*philos;
}						t_data;

void					init_forks(t_data *data);
void					init_philos(t_data *data);
long					get_time(void);
void					smart_sleep(long time, t_data *data);
void					print_status(t_philo *philo, char *msg);
int						eat(t_philo *philo);
void					*routine(void *arg);
void					*monitor(void *arg);
int						is_dead(t_data *data);
void					set_dead(t_data *data);
int						put_number(char *buf, long nb);

#endif