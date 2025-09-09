/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo_return.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:36:32 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/03 15:36:34 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	philo_cleanup(t_philo *philo, pthread_t monitor,
					pthread_t writer, t_mtxes *mtx)
{
	pthread_mutex_lock(&mtx->hold_write);
	buffer(0, -3, 0, mtx);
	pthread_mutex_unlock(&mtx->hold_write);
	if (philo->n_philos <= 1)
		return (0);
	if (philo->n_philos > 1)
	{
		if (monitor)
			pthread_join(monitor, NULL);
		if (writer)
			pthread_join(writer, NULL);
	}
	return (0);
}

int	init_philo_return(t_philo *philo, t_vals *vals, t_mtxes *mtx)
{
	static int	returned;

	pthread_mutex_lock(&mtx->hold);
	returned++;
	pthread_mutex_unlock(&mtx->hold);
	pthread_mutex_lock(&mtx->hold);
	while (returned < *philo->created)
	{
		pthread_mutex_unlock(&mtx->hold);
		usleep(1);
		pthread_mutex_lock(&mtx->hold);
	}
	pthread_mutex_unlock(&mtx->hold);
	pthread_mutex_lock(&mtx->hold_dead);
	if (*philo->died == -2 || *philo->died == 0)
		*philo->died = 1;
	pthread_mutex_unlock(&mtx->hold_dead);
	if (philo->id == 0)
		philo_cleanup(philo, vals->monitor, vals->writer, mtx);
	pthread_mutex_destroy(vals->mtx_forks[philo->id]);
	return (0);
}

int	init_check_dead(t_philo *philo, t_mtxes *mtx)
{
	pthread_mutex_lock(&mtx->hold_dead);
	if (*philo->died < 0)
	{
		philo->is_dead = 1;
		pthread_mutex_unlock(&mtx->hold_dead);
		return (1);
	}
	pthread_mutex_unlock(&mtx->hold_dead);
	return (0);
}

int	destroy_mutexes(t_mtxes *mtx)
{
	if (mtx->initialized >= 1)
		pthread_mutex_destroy(&mtx->hold);
	if (mtx->initialized >= 2)
		pthread_mutex_destroy(&mtx->hold_wait);
	if (mtx->initialized >= 3)
		pthread_mutex_destroy(&mtx->hold_write);
	if (mtx->initialized >= 4)
		pthread_mutex_destroy(&mtx->hold_dead);
	if (mtx->initialized >= 5)
		pthread_mutex_destroy(&mtx->hold_time);
	if (mtx->initialized >= 6)
		pthread_mutex_destroy(&mtx->hold_lastmeal);
	if (mtx->initialized >= 7)
		pthread_mutex_destroy(&mtx->hold_buffer);
	return (0);
}

void	create_help_threads(t_vals *vals, t_mtxes *mtx)
{
	vals->input[0] = vals->data->inpt_args->time_to_die;
	vals->input[1] = vals->data->inpt_args->n_of_philos;
	vals->monitor = monitor_create(vals, mtx);
	vals->writer = writer_create(mtx, (int *)vals->data->wait_init);
}
