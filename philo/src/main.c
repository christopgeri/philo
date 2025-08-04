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

pthread_mutex_t hold_fork;
pthread_mutex_t	hold;
static long int	start_time;

int	start_philo(t_philo *philo)
{
	if (pthread_mutex_lock(&hold_fork) != 0)
		perror("mutex lock failed: ");
	struct	timeval		tv;
	struct	timezone	tz;

	if (gettimeofday(&tv, &tz) == -1)
		perror("gettimeofday: ");
	if (start_time == 0)
		start_time = tv.tv_usec / 1000;
	if (philo->id % 2 != 0)
	{
		printf("%ld ms %d %s\n", tv.tv_usec / 1000 - start_time, philo->id, "wants to take a fork");
	}
	else
	{
		printf("%ld ms %d %s\n", tv.tv_usec / 1000 - start_time, philo->id, "is thinking");
	}
	pthread_mutex_unlock(&hold_fork);
	return (0);
}

void *init_philo(void *arg)
{
	if (pthread_mutex_lock(&hold) != 0)
		perror("mutex lock failed: ");
	static int	id;
	int		*wait_all;
	t_philo	philo;

	wait_all = (int *)arg;
	philo.id = id++;
	philo.fork = 1;
	pthread_mutex_unlock(&hold);
	while (*wait_all == 1)
		usleep(100);
	start_philo(&philo);
	return (NULL);
}

pthread_t birth_of_philo(int *wait_all)
{
	pthread_t	thread_n;

	pthread_create(&thread_n, NULL, &init_philo, (void *)wait_all);
	usleep(100);
	return(thread_n);
}

int	create_philos(t_args *args)
{
	int			philo_id;
	int			join_wait;
	int			wait_all;
	pthread_t	*thread_ids;

	thread_ids = (pthread_t *) malloc(args->n_of_philos * sizeof(pthread_t));
	if (!thread_ids)
		return (1);
	philo_id = 0;
	wait_all = 1;
	pthread_mutex_init(&hold_fork, NULL);
	pthread_mutex_init(&hold, NULL);
	printf("START OF SIMULATION\n");
	while (philo_id < args->n_of_philos)
	{
		thread_ids[philo_id] = birth_of_philo(&wait_all);
		philo_id++;
	}
	wait_all = 0;
	join_wait = 0;
	printf("\nstart_time: %ld\n", start_time);
	while (join_wait < philo_id)
	{
		pthread_join(thread_ids[join_wait], NULL);
		printf("Philo Nr. %d has finished\n", join_wait);
		join_wait++;
	}
	printf("\nall threads returned!\n");
	free(thread_ids);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_args	args;

	if (argc != 6)
	{
		write(2, "exactly 6 arguments required!\n", 30);
		return (1);
	}
	args.n_of_philos = ft_atoi(argv[1]);
	args.time_to_die = ft_atoi(argv[2]);
	args.time_to_eat = ft_atoi(argv[3]);
	args.time_to_sleep = ft_atoi(argv[4]);
	args.n_of_meals = ft_atoi(argv[5]);
	printf("num philos = %d\ntime_eat = %d ms\ntime_die = %d ms\ntime_to_sleep = %d\nnum_meals = %d\n\n",
		args.n_of_philos, args.time_to_eat, args.time_to_die, args.time_to_sleep, args.n_of_meals);
	create_philos(&args);
	return (0);
}
