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

int return_philo(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks, int **last_meals);

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
	static int				*forks;
	static pthread_mutex_t	**mtx_forks;
	static int				**last_meals;
	static int				input[2];

	data = (t_thread_data *) arg;
	if (id == 0)
	{
		forks = (int *) malloc(data->inpt_args->n_of_philos * sizeof(int)); // allocate fork for every philo
		mtx_forks = (pthread_mutex_t **) malloc(data->inpt_args->n_of_philos * sizeof(pthread_mutex_t *)); // allocate mutexes for every fork
		last_meals = (int **) malloc(data->inpt_args->n_of_philos * sizeof(int *));
		input[0] = data->inpt_args->time_to_die;
		input[1] = data->inpt_args->n_of_philos;
	}
	forks[id] = 1;
	mtx_forks[id] = &mtx_lock_fork;
	pthread_mutex_lock(&hold_lastmeal);
	pthread_mutex_init(mtx_forks[id], NULL);
	pthread_mutex_unlock(&hold_lastmeal);
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
	pthread_mutex_lock(&hold_lastmeal);
	last_meals[id] = &philo.last_meal;
	pthread_mutex_unlock(&hold_lastmeal);
	philo.died = (int *)data->wait_init;
	id++;
	pthread_mutex_unlock(&hold);
	pthread_mutex_lock(&hold_wait);
	if (philo.id == 0 && philo.n_philos > 1) // create the monitor thread
		monitor_create(last_meals, forks, mtx_forks, wait_all, input);
	pthread_mutex_unlock(&hold_wait);
	if (philo.id % 2 == 0 && philo.n_philos > 1) // start the even threads delayed
	{
		// pthread_mutex_lock(&hold);
		pthread_mutex_lock(&hold_write);
		printf("%d %d %s", 0, philo.id, "is thinking\n");
		pthread_mutex_unlock(&hold_write);
		// pthread_mutex_unlock(&hold);
		usleep(philo.eat * 0.1);
	}
	start_philo(&philo, forks, mtx_forks); // start philo loop
	// pthread_mutex_lock(&hold);
	pthread_mutex_lock(&hold_dead);
	if (*wait_all != -1)
		*wait_all = 1;
	pthread_mutex_unlock(&hold_dead);
	// pthread_mutex_unlock(&hold);
	return_philo(&philo, forks, mtx_forks, last_meals);
	return (NULL);
}

int return_philo(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks, int **last_meals)
{
	static int	returned;

	pthread_mutex_lock(&hold);
	returned++;
	pthread_mutex_unlock(&hold);
	pthread_mutex_lock(&hold);
	while (returned < philo->n_philos) // waits till all threads returned;
	{
		pthread_mutex_unlock(&hold);
		usleep(1000);
		pthread_mutex_lock(&hold);
	}
	// pthread_mutex_lock(&hold);
	// printf("%d had %d meals %d\n", philo->id, philo->meal_cnt, *philo->died);
	pthread_mutex_unlock(&hold);
	if (philo->id == 0) // free the allocated arrays
	{
		free(forks);
		free(mtx_forks);
		free(last_meals);
	}
	return (returned);
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
	pthread_mutex_lock(&hold_time);
	sec = tv.tv_sec - seconds;
	usec = tv.tv_usec - micro_seconds;
	pthread_mutex_unlock(&hold_time);
	if (usec < 0)
	{
		sec -= 1;
		usec += 1000000;
	}
	current_time = sec * 1000 + usec / 1000;
	return (current_time);
}