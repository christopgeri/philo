/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_think.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:25:02 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/01 14:25:07 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	philo_died(t_philo *philo, t_mtxes *mtx);

int	philo_think(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	philo_died(philo, mtx);
	if (philo->n_philos % 2 == 0
		&& (philo->die - (philo->eat + philo->sleep)) <= 1000)
		philo_start(philo, mtx_forks, mtx);
	else
	{
		pthread_mutex_lock(&mtx->hold_write);
		philo->current_time = timestamp(mtx);
		if (philo->is_dead == 1)
		{
			pthread_mutex_unlock(&mtx->hold_write);
			return (-1);
		}
		buffer(philo->current_time, philo->id, 5, mtx);
		pthread_mutex_unlock(&mtx->hold_write);
		if (philo->n_philos == 3 && philo->eat > philo->sleep)
			usleep((philo->eat * 2) - philo->sleep - 500);
		else if (philo->n_philos % 2 == 1 && philo->id % 2 == 1
			&& philo->eat >= philo->sleep)
			usleep(philo->eat * 0.9);
		philo_start(philo, mtx_forks, mtx);
	}
	return (0);
}

int	philo_died(t_philo *philo, t_mtxes *mtx)
{
	pthread_mutex_lock(&mtx->hold_dead);
	if (*philo->died < 0)
		philo->is_dead = 1;
	pthread_mutex_unlock(&mtx->hold_dead);
	return (0);
}
