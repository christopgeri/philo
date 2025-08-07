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

int philo_values(t_philo *philo, t_thread_data *data);

/*
Init a philo and waits till all threads are created
init an array with addresses of philo structs
*/
void *init_philo(void *arg)
{
	if (pthread_mutex_lock(&hold) != 0)
		perror("mutex lock failed: ");
	static int				id;
	int						*wait_all;
	t_philo					philo;
	static t_thread_data	*data;
	static void				**philos_array;
	pthread_mutex_t			mtx_lock_fork;

	data = (t_thread_data *) arg;
	if (id == 0)
	{
		philos_array = (void **) malloc((data->inpt_args->n_of_philos + 1) * sizeof(void *));
		philos_array[data->inpt_args->n_of_philos] = NULL;
	}
	philos_array[id++] = &philo;
	wait_all = (int *)data->wait_init;
	philo.id = id;
	philo.fork = 1;
	philo.eat = data->inpt_args->time_to_eat * 1000;
	philo.die = data->inpt_args->time_to_die * 1000;
	philo.sleep = data->inpt_args->time_to_sleep * 1000;
	philo.meals = data->inpt_args->n_of_meals;
	philo.meal_cnt = 0;
	philo.n_philos = data->inpt_args->n_of_philos;
	philo.last_meal = 0;
	philo.lock_fork = &mtx_lock_fork;
	pthread_mutex_init(philo.lock_fork, NULL);
	pthread_mutex_unlock(&hold);
	while (*wait_all == 1)
		usleep(10);
	if (philo.id % 2 != 0)
		usleep(philo.eat);
	start_philo(&philo, philos_array);
	// free(philos_array);
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