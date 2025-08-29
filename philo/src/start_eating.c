/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_eating.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgeringe <cgeringe@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:44:05 by cgeringe          #+#    #+#             */
/*   Updated: 2025/08/05 13:44:08 by cgeringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

/*
philo starts eating for the given time
and then calls the sleeping function
*/
int start_eating(t_philo *philo, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(&hold_write);
	pthread_mutex_lock(&hold_lastmeal);
	philo->last_meal = time_for_timestamp();
	philo->current_time = philo->last_meal;
	pthread_mutex_unlock(&hold_lastmeal);
	buffer(philo->current_time, philo->id, 3); //write_status(philo->current_time, philo->id, " is eating\n");
	pthread_mutex_unlock(&hold_write);
	pthread_mutex_lock(&hold_dead);
	if (philo->is_dead == 1)
	{
		unlock_fork(philo, mtx_forks);
		pthread_mutex_unlock(&hold_dead);
		return (-1);
	}
	pthread_mutex_unlock(&hold_dead);
	while (time_for_timestamp() - philo->current_time < philo->eat - 10)
		usleep(1);
	// usleep(philo->eat - 50);
	philo->meal_cnt++;
	unlock_fork(philo, mtx_forks);
	start_sleeping(philo, mtx_forks);
	return (0);
}
