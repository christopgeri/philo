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

int	take_fork(t_philo *philo)
{
	pthread_mutex_t	hold_fork;
	struct timeval start;
	double mili;

	mili = 0;
	pthread_mutex_init(&hold_fork, NULL);
	pthread_mutex_lock(&hold_fork);
	gettimeofday(&start, NULL);
	mili = start.tv_usec / 1000;
	if (philo->fork == 0)
	{
		memset(&philo->eat, mili, sizeof(int));
		printf("%d ms %d %s\n", philo->eat, philo->id, "has taken a fork");
		// memset(&mili, 0, 1);
		// ft_putnbr_fd(philo.id, 1);
		// write(1, "no fork\n", 8);
	}
	else
	{
		memset(&philo->think, mili, sizeof(int));
		// printf("%d ms %d %s\t(%d)\n", philo.think,  philo.id, "is thinking", check);
		printf("%d ms %d %s\n", philo->think, philo->id, "is thinking");
		// memset(&mili, 0, 1);
		// ft_putnbr_fd(philo.id, 1);
		// write(1, "fork\n", 5);
	}
	// free(philo);
	pthread_mutex_unlock(&hold_fork);
	return (0);
}

void *hello(void *arg)
{
	pthread_mutex_t	hold;
	int				*n;
	t_philo			*philo;

	pthread_mutex_init(&hold, NULL);
	pthread_mutex_lock(&hold);
	n = (int *)arg;
	philo = (t_philo *) malloc(1 * sizeof(t_philo));
	if (*n % 2 == 0)
	{
		memset(&philo->id, *n, sizeof(int));
		philo->fork = 0;
		take_fork(philo);
	}
	else
	{
		memset(&philo->id, *n, sizeof(int));
		philo->fork = 1;
		take_fork(philo);
	}
	pthread_mutex_unlock(&hold);
	return (NULL);
}

pthread_t birth_of_philo(int philo_id)
{
	pthread_t	thread_n;
	static int	id;

	philo_id = 0;
	pthread_create(&thread_n, NULL, &hello, (void *)&id);
	usleep(100);
	id++;
	return(thread_n);
}

int	create_philos(t_args *args)
{
	int			philo_id;
	int			join_wait;
	pthread_t	*thread_ids;
	
	thread_ids = (pthread_t *) malloc(args->n_of_philos * sizeof(pthread_t));
	if (!thread_ids)
		return (1);
	philo_id = 0;
	while (philo_id < args->n_of_philos)
	{
		thread_ids[philo_id] = birth_of_philo(philo_id);
		philo_id++;
	}
	join_wait = 0;
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
	usleep(100000);
	return (0);
}
