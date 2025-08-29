/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:43:44 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:43:49 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int return_philo(t_philo *philo);

/*
Init a philo and waits till all threads are created
init an array with addresses of philo structs
*/
// int wait_philo(t_philo *philo, pthread_mutex_t **mtx_forks)
// {
// 	if (philo->n_philos % 2 == 1 && philo->id % 2 == 0 && philo->n_philos > 1)
// 	{
// 		if (philo->id == philo->n_philos - 1)
// 			usleep(philo->eat * 1.5);
// 		usleep(philo->eat * 0.9);
// 	}
// 	else
// 	{
// 		if (philo->id % 2 == 0 && philo->n_philos > 1)
// 			usleep(philo->eat / 2);
// 		else if (philo->n_philos % 2 == 0 && philo->id % 2 == 1 && philo->n_philos > 1 && philo->id < philo->n_philos / 2)
// 			usleep(100);
// 	}
// 	start_philo(philo, mtx_forks);
// 	return (0);
// }

void *init_philo(void *arg)
{
	if (pthread_mutex_lock(&hold) != 0)
		perror("mutex lock failed: ");
	t_philo					philo;
	int						*wait_all;
	pthread_mutex_t			mtx_lock_fork;
	static int				id;
	static t_thread_data	*data;
	static pthread_mutex_t	*mtx_forks[200];
	static int				*last_meals[200];
	static int				input[2];
	static pthread_t		monitor;

	data = (t_thread_data *) arg;
	if (id == 0)
	{
		// mtx_forks = (pthread_mutex_t **) malloc(data->inpt_args->n_of_philos * sizeof(pthread_mutex_t *)); // allocate mutexes for every fork
		input[0] = data->inpt_args->time_to_die;
		input[1] = data->inpt_args->n_of_philos;
	}
	mtx_forks[id] = &mtx_lock_fork;
	wait_all = (int *)data->wait_init;
	philo.id = id;
	philo.fork = 0;
	philo.eat = data->inpt_args->time_to_eat * 1000;
	philo.die = data->inpt_args->time_to_die * 1000;
	philo.sleep = data->inpt_args->time_to_sleep * 1000;
	philo.meals = data->inpt_args->n_of_meals;
	philo.meal_cnt = 0;
	philo.n_philos = data->inpt_args->n_of_philos;
	philo.last_meal = 0;
	philo.current_time = 0;
	philo.is_dead = 0;
	pthread_mutex_lock(&hold_lastmeal);
	pthread_mutex_init(mtx_forks[id], NULL);
	last_meals[id] = &philo.last_meal;
	pthread_mutex_unlock(&hold_lastmeal);
	philo.left = id;
	if (id == 0)
		philo.right = data->inpt_args->n_of_philos - 1;
	else
		philo.right = id - 1;
	philo.died = (int *)data->wait_init;
	if (philo.id == 0 && philo.n_philos > 1) // create the monitor thread
		monitor = monitor_create(last_meals, mtx_forks, wait_all, input);
	id++;
	pthread_mutex_unlock(&hold);
	pthread_mutex_lock(&hold_time);
	while (seconds == 0 && micro_seconds == 0) 
	{
		pthread_mutex_unlock(&hold_time);
		pthread_mutex_lock(&hold_time);
	}
	pthread_mutex_unlock(&hold_time);
	if (philo.n_philos % 2 == 1 && philo.id % 2 == 0 && philo.n_philos > 1)// start the even threads delayed
	{
		if (philo.id == philo.n_philos - 1)
			usleep(philo.eat * 0.9);
		usleep(philo.eat * 0.4);
	}
	else if (philo.id % 2 == 0 && philo.n_philos > 1)
		usleep(philo.eat / 2);
	start_philo(&philo, mtx_forks); // start philo loop
	return_philo(&philo); // wait for return of all philos
	pthread_mutex_lock(&hold_dead);
	if (*wait_all == -2 || *wait_all == 0)
		*wait_all = 1;
	pthread_mutex_unlock(&hold_dead);
	usleep(10000);
	if (philo.id == 0) // free allocated arrays
	{
		pthread_mutex_lock(&hold_dead);
		pthread_mutex_lock(&hold_write);
			buffer(-1, -1, -1);
		pthread_mutex_unlock(&hold_write);
		pthread_mutex_unlock(&hold_dead);
		pthread_join(monitor, NULL);
		// free(mtx_forks);
		// free(last_meals);
	}
	return (NULL);
}

int return_philo(t_philo *philo)
{
	pthread_mutex_lock(&hold);
	static int	returned;

	returned++;
	pthread_mutex_unlock(&hold);
	pthread_mutex_lock(&hold);
	while (returned < philo->n_philos) // waits till all threads returned;
	{
		pthread_mutex_unlock(&hold);
		usleep(1000);
		pthread_mutex_lock(&hold);
	}
	pthread_mutex_unlock(&hold);
	return (0);
}

int time_for_timestamp(void)
{
	pthread_mutex_lock(&hold_time);
	int		current_time;
	struct	timeval		tv;
	struct	timezone	tz;
	int		sec;
	int		usec;

	if (micro_seconds == 0 && seconds == 0)
	{
		if (gettimeofday(&tv, &tz) == -1)
			perror("gettimeofday: ");
		micro_seconds = tv.tv_usec;
		seconds = tv.tv_sec;
	}
	current_time = 0;
	gettimeofday(&tv, &tz);
	sec = tv.tv_sec - seconds;
	usec = tv.tv_usec - micro_seconds;
	if (usec < 0)
	{
		sec -= 1;
		usec += 1000000;
	}
	current_time = (sec * 1000000) + usec;
	pthread_mutex_unlock(&hold_time);
	return (current_time);
}
