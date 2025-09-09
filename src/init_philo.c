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

void	init_philo_values(t_philo *philo, t_vals *vals);
int		delay_start(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx);
int		philo_wait(t_philo *philo, t_mtxes *mtx);
int		calc_delay(t_philo *philo);

void	*init_philo(void *arg)
{
	static t_vals			vals;
	t_philo					philo;
	pthread_mutex_t			mtx_lock_fork;
	t_mtxes					*mtx;

	pthread_mutex_lock(&((t_thread_data *)arg)->mtx->hold);
	mtx = ((t_thread_data *)arg)->mtx;
	vals.data = (t_thread_data *) arg;
	init_philo_values(&philo, &vals);
	pthread_mutex_lock(&mtx->hold_lastmeal);
	vals.mtx_forks[philo.id] = &mtx_lock_fork;
	if (pthread_mutex_init(vals.mtx_forks[philo.id], NULL) != 0)
	{
		pthread_mutex_lock(&mtx->hold_dead);
		*philo.died = -1;
		pthread_mutex_unlock(&mtx->hold_dead);
	}
	vals.last_meals[philo.id] = &philo.last_meal;
	pthread_mutex_unlock(&mtx->hold_lastmeal);
	if (philo.id == 0 && philo.n_philos > 1)
		create_help_threads(&vals, mtx);
	pthread_mutex_unlock(&mtx->hold);
	delay_start(&philo, vals.mtx_forks, mtx);
	init_philo_return(&philo, &vals, mtx);
	return (NULL);
}

void	init_philo_values(t_philo *philo, t_vals *vals)
{
	static int				id;
	static int				initialized;

	memset(philo, 0, sizeof(t_philo));
	philo->id = id;
	philo->eat = vals->data->inpt_args->time_to_eat * 1000;
	philo->die = vals->data->inpt_args->time_to_die * 1000;
	philo->sleep = vals->data->inpt_args->time_to_sleep * 1000;
	philo->meals = vals->data->inpt_args->n_of_meals;
	philo->n_philos = vals->data->inpt_args->n_of_philos;
	philo->left = id;
	philo->created = &initialized;
	if (id == 0)
		philo->right = vals->data->inpt_args->n_of_philos - 1;
	else
		philo->right = id - 1;
	philo->died = vals->data->wait_init;
	philo->delay = 0;
	calc_delay(philo);
	id++;
	initialized++;
}

int	calc_delay(t_philo *philo)
{
	if (philo->id < philo->n_philos / 2)
		philo->delay = philo->id * 10;
	else
		philo->delay = (philo->n_philos - philo->id - 1) * 10;
	philo->delay = philo->delay;
	if (philo->die < philo->eat + philo->sleep)
	{
		if (philo->id % 2 == 0)
			philo->delay += 1000;
	}
	return (0);
}

int	delay_start(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	philo_wait(philo, mtx);
	if (philo->die < philo->eat + philo->sleep)
		usleep(philo->delay);
	else if (philo->n_philos % 2 == 1 && philo->n_philos > 1)
	{
		if (philo->id == philo->n_philos - 1)
			usleep(philo->eat * 1 + philo->delay);
		else if (philo->id % 2 == 0)
			usleep(philo->eat * 0.4 + philo->delay);
		else
			usleep(philo->delay);
	}
	else if (philo->n_philos % 2 == 0 && philo->n_philos > 1)
	{
		if (philo->id % 2 == 0)
			usleep((philo->eat / 2) + philo->delay);
		else
			usleep(philo->delay);
	}
	if (init_check_dead(philo, mtx) == 1)
		return (1);
	philo_start(philo, mtx_forks, mtx);
	return (0);
}

int	philo_wait(t_philo *philo, t_mtxes *mtx)
{
	pthread_mutex_lock(&mtx->hold_dead);
	while (*philo->died == 1)
	{
		pthread_mutex_unlock(&mtx->hold_dead);
		usleep(10);
		pthread_mutex_lock(&mtx->hold_dead);
	}
	pthread_mutex_unlock(&mtx->hold_dead);
	return (0);
}
