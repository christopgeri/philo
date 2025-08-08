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

/* TESTCASES
./philo 1 800 200 200 		!eat -> die
./philo 5 800 200 200 		!die
./philo 5 800 200 200 7		!die for meals
./philo 4 410 200 200 		!die
./philo 1 310 200 100 		1 dies
*/
pthread_mutex_t hold_eating;
pthread_mutex_t hold_fork;
pthread_mutex_t	hold_sleep;
pthread_mutex_t	hold;
long int		micro_seconds;
long int		seconds;
long int		elapsed_ms;

/*
calls pthread_create and returns the pthread_t of new thread
*/
pthread_t birth_of_philo(int *wait_all, t_args *args)
{
	pthread_t	thread_n;
	t_thread_data data;

	data.inpt_args = args;
	data.wait_init = wait_all;
	pthread_create(&thread_n, NULL, &init_philo, (void *)&data);
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

	thread_ids = (pthread_t *) malloc((args->n_of_philos) * sizeof(pthread_t));
	if (!thread_ids)
		return (1);
	philo_id = 0;
	wait_all = 1;
	pthread_mutex_init(&hold_fork, NULL);
	pthread_mutex_init(&hold, NULL);
	pthread_mutex_init(&hold_eating, NULL);
	pthread_mutex_init(&hold_sleep, NULL);
	while (philo_id < args->n_of_philos)
	{
		thread_ids[philo_id] = birth_of_philo(&wait_all, args);
		philo_id++;
	}
	wait_all = 0;
	// printf("START OF SIMULATION\n");
	join_wait = 0;
	while (join_wait < philo_id)
	{
		pthread_join(thread_ids[join_wait], NULL);
		join_wait++;
	}
	// while (join_wait < philo_id && wait_all == 0)
	// {
		// pthread_join(thread_ids[join_wait], NULL);
		// join_wait++;
	// }
	// if (wait_all == 0)
	// {
	// 	while (join_wait < philo_id)
	// 		pthread_detach(thread_ids[join_wait++]);
	// }
	// printf("\nall threads returned!\n");
	free(thread_ids);
	return (0);
}

/*
converts arguments to int values
and calls creat philos()
*/
int	main(int argc, char *argv[])
{
	t_args	args;

	if (argc < 5 || argc > 6)
	{
		write(2, "minimum 5 arguments required!\n", 30);
		return (1);
	}
	args.n_of_philos = ft_atoi(argv[1]);
	args.time_to_die = ft_atoi(argv[2]);
	args.time_to_eat = ft_atoi(argv[3]);
	args.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args.n_of_meals = ft_atoi(argv[5]);
	else
		args.n_of_meals = 1000;
	// printf("ARGS\nnum philos = %d\ntime_eat = %d ms\ntime_die = %d ms\ntime_to_sleep = %d\nnum_meals = %d\n\n",
	// 	args.n_of_philos, args.time_to_eat, args.time_to_die, args.time_to_sleep, args.n_of_meals);
	create_philos(&args);
	return (0);
}

/*
	forks on table;
	mutex a fork;
*/