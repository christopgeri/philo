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

/*
Init a philo and waits till all threads are created
init an array with addresses of philo structs
*/
void *init_philo(void *arg)
{
	if (pthread_mutex_lock(&hold) != 0)
		perror("mutex lock failed: ");
	t_philo					philo;
	int						*wait_all;
	pthread_mutex_t			mtx_lock_fork;
	static int				id;
	static t_thread_data	*data;
	static void				**philos_array;
	static int				*forks;
	static pthread_mutex_t	**mtx_forks;
	static int				returned;
	data = (t_thread_data *) arg;
	if (id == 0)
	{
		philos_array = (void **) malloc((data->inpt_args->n_of_philos + 1) * sizeof(void *));
		philos_array[data->inpt_args->n_of_philos] = NULL;
		forks = (int *) malloc(data->inpt_args->n_of_philos * sizeof(int));
		mtx_forks = (pthread_mutex_t **) malloc(data->inpt_args->n_of_philos * sizeof(pthread_mutex_t *));
	}
	forks[id] = 1;
	mtx_forks[id] = &mtx_lock_fork;
	pthread_mutex_init(mtx_forks[id], NULL);
	philos_array[id] = &philo;
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
	philo.died = (int *)data->wait_init;
	// printf("%d &= %p\n", id, &philo);
	id++;
	pthread_mutex_unlock(&hold);
	while (*wait_all == 1)
		usleep(10);
	if (philo.id % 2 == 0)
		usleep(philo.eat);
	start_philo(&philo, philos_array, forks, mtx_forks);
	returned++;
	while (returned < philo.n_philos)
		usleep(1000);
	if (philo.id == 0)
	{
		free(philos_array);
		free(forks);
		free(mtx_forks);
	}
	return (NULL);
}

int philo_values(t_philo *philo, t_thread_data *data)
{
	philo->fork = 1;
	philo->eat = data->inpt_args->time_to_eat * 1000;
	philo->die = data->inpt_args->time_to_die * 1000;
	philo->sleep = data->inpt_args->time_to_sleep * 1000;
	philo->meals = data->inpt_args->n_of_meals;
	return (0);
}

int time_for_timestamp(void)
{
	int current_time;
	struct	timeval		tv;
	struct	timezone	tz;
	int		sec;
	int		usec;

	current_time = 0;
	gettimeofday(&tv, &tz);
	sec = tv.tv_sec - seconds;
	usec = tv.tv_usec - micro_seconds;
	if (usec < 0)
	{
		sec -= 1;
		usec += 1000000;
	}
	current_time = sec * 1000 + usec / 1000;
	return (current_time);
}