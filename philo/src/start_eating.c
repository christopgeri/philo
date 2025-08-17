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
int start_eating(t_philo *philo, int *forks, pthread_mutex_t **mtx_forks)
{
	pthread_mutex_lock(&hold_eating);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died == -1)
	{
		pthread_mutex_unlock(&hold_dead);
		unlock_fork(philo, forks, mtx_forks);
		pthread_mutex_unlock(&hold_eating);
		return (-1);
	}
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_lock(&hold_lastmeal);
	philo->last_meal = time_for_timestamp();
	pthread_mutex_unlock(&hold_lastmeal);
	pthread_mutex_lock(&hold_write);
	pthread_mutex_lock(&hold_dead);
	if (*philo->died != -1)
		printf("%d %d %s\n", philo->last_meal, philo->id, "is eating");
	pthread_mutex_unlock(&hold_dead);
	pthread_mutex_unlock(&hold_write);
	philo->meal_cnt++;
	pthread_mutex_unlock(&hold_eating);
	usleep(philo->eat);
	pthread_mutex_lock(&hold_eating);
	unlock_fork(philo, forks, mtx_forks);
	pthread_mutex_unlock(&hold_eating);
	start_sleeping(philo, forks, mtx_forks);
	return (0);
}
