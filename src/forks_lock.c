/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 13:55:30 by cgeringe          #+#    #+#             */
/*   Updated: 2025/09/01 13:57:11 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	lock_left(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx);
int	lock_right(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx);

int	forks_lock(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	if (philo->id % 2 == 0)
	{
		if (lock_right(philo, mtx_forks, mtx) == 1)
			return (1);
		if (lock_left(philo, mtx_forks, mtx) == 1)
			return (1);
	}
	else
	{
		if (lock_left(philo, mtx_forks, mtx) == 1)
			return (1);
		if (lock_right(philo, mtx_forks, mtx) == 1)
			return (1);
	}
	return (2);
}

int	lock_left(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	pthread_mutex_lock(mtx_forks[philo->left]);
	pthread_mutex_lock(&mtx->hold_write);
	philo->current_time = timestamp(mtx);
	if (philo->is_dead == 0)
	{
		buffer(philo->current_time, philo->id, 1, mtx);
		pthread_mutex_unlock(&mtx->hold_write);
	}
	else
	{
		pthread_mutex_unlock(&mtx->hold_write);
		pthread_mutex_unlock(mtx_forks[philo->left]);
		if (philo->id % 2 == 0)
			pthread_mutex_unlock(mtx_forks[philo->right]);
		return (1);
	}
	return (0);
}

int	lock_right(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	pthread_mutex_lock(mtx_forks[philo->right]);
	pthread_mutex_lock(&mtx->hold_write);
	philo->current_time = timestamp(mtx);
	if (philo->is_dead == 0)
	{
		buffer(philo->current_time, philo->id, 2, mtx);
		pthread_mutex_unlock(&mtx->hold_write);
	}
	else
	{
		pthread_mutex_unlock(&mtx->hold_write);
		pthread_mutex_unlock(mtx_forks[philo->right]);
		if (philo->id % 2 != 0)
			pthread_mutex_unlock(mtx_forks[philo->left]);
		return (1);
	}
	return (0);
}
