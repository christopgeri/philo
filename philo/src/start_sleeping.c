/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sleeping.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:37 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:40 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
philo starts sleepin for given time
*/
int start_sleeping(t_philo *philo, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(&hold_write);
	philo->current_time = time_for_timestamp();
	buffer(philo->current_time, philo->id, 4); //write_status(philo->current_time, philo->id, " is sleeping\n");
	pthread_mutex_unlock(&hold_write);
	pthread_mutex_lock(&hold_dead);
	if (philo->is_dead == 1)
	{
		pthread_mutex_unlock(&hold_dead);
		return (-1);
	}
	pthread_mutex_unlock(&hold_dead);
	while (time_for_timestamp() - philo->current_time < philo->sleep - 10)
		usleep(1);
	if (philo->meal_cnt == philo->meals)
	{
		pthread_mutex_lock(&hold_lastmeal);
		philo->last_meal = -1;
		pthread_mutex_unlock(&hold_lastmeal);
		return (-1);
	}
// start_thinking
	pthread_mutex_lock(&hold_dead);
	if (*philo->died < 0)
		philo->is_dead = 1;
	pthread_mutex_unlock(&hold_dead);
	if (philo->n_philos % 2 == 0 && (philo->die - (philo->eat + philo->sleep)) <= 1000)
		start_philo(philo, mtx_forks);
	else
	{
		pthread_mutex_lock(&hold_write);
		philo->current_time = time_for_timestamp();
		if (philo->is_dead == 0)
		{
			buffer(philo->current_time, philo->id, 5);
			pthread_mutex_unlock(&hold_write);
		}
		else
		{
			pthread_mutex_unlock(&hold_write);
			return (-1);
		}
		if (philo->n_philos % 2 == 1 && philo->id % 2 == 1)
			usleep(philo->eat * 0.9);
		start_philo(philo, mtx_forks);
	}
	return (0);
}
