/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:37 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:40 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	sleep_check_dead(t_philo *philo, t_mtxes *mtx);

int	philo_sleep(t_philo *philo, pthread_mutex_t **mtx_forks, t_mtxes *mtx)
{
	pthread_mutex_lock(&mtx->hold_write);
	philo->current_time = timestamp(mtx);
	buffer(philo->current_time, philo->id, 4, mtx);
	pthread_mutex_unlock(&mtx->hold_write);
	pthread_mutex_lock(&mtx->hold_dead);
	if (philo->is_dead == 1)
	{
		pthread_mutex_unlock(&mtx->hold_dead);
		return (-1);
	}
	pthread_mutex_unlock(&mtx->hold_dead);
	if (sleep_check_dead(philo, mtx) == 1)
		return (1);
	if (philo->meals != -1 && philo->meal_cnt == philo->meals)
	{
		pthread_mutex_lock(&mtx->hold_lastmeal);
		philo->last_meal = -1;
		pthread_mutex_unlock(&mtx->hold_lastmeal);
		return (-1);
	}
	philo_think(philo, mtx_forks, mtx);
	return (0);
}

int	sleep_check_dead(t_philo *philo, t_mtxes *mtx)
{
	philo->sleep_loops = 0;
	if (philo->sleep > 1000000)
		philo->sleep_loops = philo->sleep / 1000000;
	if (philo->sleep_loops < 1)
		usleep(philo->sleep);
	else
	{
		while (philo->sleep_loops > 0)
		{
			usleep(1000000);
			pthread_mutex_lock(&mtx->hold_dead);
			if (*philo->died == -1)
			{
				pthread_mutex_unlock(&mtx->hold_dead);
				philo->is_dead = -1;
				return (1);
			}
			pthread_mutex_unlock(&mtx->hold_dead);
			philo->sleep_loops--;
		}
	}
	return (0);
}
