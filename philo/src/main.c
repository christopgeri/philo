/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:24:07 by cgeringe          #+#    #+#             */
/*   Updated: 2025/07/29 18:33:27 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"


pthread_mutex_t hold_eating;
pthread_mutex_t hold_fork;
pthread_mutex_t	hold_sleep;
pthread_mutex_t	hold;
pthread_mutex_t	hold_birth;
pthread_mutex_t	hold_wait;
pthread_mutex_t	hold_write;
pthread_mutex_t	hold_dead;
pthread_mutex_t	hold_time;
pthread_mutex_t hold_lastmeal;

long int		micro_seconds;
long int		seconds;
long int		elapsed_ms;

/*
calls pthread_create and returns the pthread_t of new thread
*/
pthread_t birth_of_philo(t_thread_data *data)
{
	// pthread_mutex_lock(&hold_birth);
	pthread_t	thread_n;

	pthread_create(&thread_n, NULL, &init_philo, (void *)data);
	// pthread_mutex_unlock(&hold_birth);
	usleep(100);
	return(thread_n);
}

/*
Allocates array of pthread_t with n_of_philos
and waits till threads are finished
*/
int	create_philos(t_args *args)
{
	int			philo_id;
	int			join_wait;
	int			wait_all;
	pthread_t	*thread_ids;
	t_thread_data data;
	// t_mtx		mtx;

	thread_ids = (pthread_t *) malloc((args->n_of_philos) * sizeof(pthread_t));
	if (!thread_ids)
		return (1);
	philo_id = 0;
	wait_all = 1;
	data.inpt_args = args;
	data.wait_init = &wait_all;
	pthread_mutex_init(&hold_fork, NULL);
	pthread_mutex_init(&hold, NULL);
	pthread_mutex_init(&hold_eating, NULL);
	pthread_mutex_init(&hold_sleep, NULL);
	pthread_mutex_init(&hold_wait, NULL);
	pthread_mutex_init(&hold_write, NULL);
	pthread_mutex_init(&hold_dead, NULL);
	pthread_mutex_init(&hold_time, NULL);
	pthread_mutex_init(&hold_lastmeal, NULL);
	pthread_mutex_lock(&hold_wait);
	while (philo_id < args->n_of_philos)
	{
		thread_ids[philo_id] = birth_of_philo(&data);
		philo_id++;
	}
	wait_all = 0;
	pthread_mutex_unlock(&hold_wait);
	// pthread_mutex_unlock(&hold_wait);
	join_wait = 0;
	while (join_wait < philo_id)
	{
		pthread_join(thread_ids[join_wait], NULL);
		join_wait++;
	}
	pthread_mutex_destroy(&hold_fork);
	pthread_mutex_destroy(&hold);
	pthread_mutex_destroy(&hold_eating);
	pthread_mutex_destroy(&hold_sleep);
	// pthread_mutex_destroy(&hold_dead);
	free(thread_ids);
	return (0);
}

/*
converts arguments to int values
and calls creat philos()
*/
int	main(int argc, char *argv[])
{
	t_args	*args;

	if (argc < 5 || argc > 6)
	{
		write(2, "minimum 5 arguments required!\n", 30);
		return (1);
	}
	args = (t_args *) malloc(1 * sizeof(t_args));
	if (!args)
		return (1);
	args->n_of_philos = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->n_of_meals = ft_atoi(argv[5]);
	else
		args->n_of_meals = 1000;
	create_philos(args);
	free(args);
	return (0);
}
